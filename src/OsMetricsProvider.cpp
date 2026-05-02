#include "OsMetricsProvider.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std::filesystem;

OsMetricsProvider::OsMetricsProvider() :
    mPrevCpuTotal(0),
    mPrevCpuIdle(0)
{
}

OsMetricsProvider::~OsMetricsProvider()
{
}

std::string OsMetricsProvider::getCpuModelName()
{
    std::ifstream file("/proc/cpuinfo");
    if (!file.is_open())
        return "Unknown CPU";

    std::string line;

    while (std::getline(file, line))
    {
        if (line.find("model name") != std::string::npos)
        {
            size_t colonPos = line.find(":");

            if (colonPos != std::string::npos)
                return line.substr(colonPos + 1);
        }
    }

    return "Unknown CPU";
}

CpuStats OsMetricsProvider::queryCpuStats()
{
    CpuStats stats;

    std::ifstream file("/proc/stat");
    if (!file.is_open())
        return CpuStats();

    std::string label;
    uint64_t user = 0;
    uint64_t nice = 0;
    uint64_t system = 0;
    uint64_t idle = 0;
    uint64_t iowait = 0;
    uint64_t irq = 0;
    uint64_t softirq = 0;
    uint64_t steal = 0;

    file >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;    

    // Calculate totals
    uint64_t currentIdle = idle + iowait;
    uint64_t currentTotal = user + nice + system + idle + iowait + irq + softirq + steal;

    // Calculate the change since the last check
    uint64_t totalDiff = currentTotal - mPrevCpuTotal;
    uint64_t idleDiff = currentIdle - mPrevCpuIdle;

    // Save current values for the next run
    mPrevCpuTotal = currentTotal;
    mPrevCpuIdle = currentIdle;

    if (totalDiff > 0)
        stats.percent = ((double)(totalDiff - idleDiff) / totalDiff) * 100.0;

    return stats;
}

MemoryStats OsMetricsProvider::queryMemoryStats()
{
    MemoryStats stats;

    std::ifstream file("/proc/meminfo");
    if (!file.is_open())
        return MemoryStats();

    std::string key;
    std::string unit;
    uint64_t totalKb = 0;
    uint64_t availKb = 0;
    uint64_t value = 0;

    while (file >> key >> value >> unit)
    {
        if (key == "MemTotal:")
            totalKb = value;
        else if (key == "MemAvailable:")
            availKb = value;
    }

    stats.percent = ((double)(totalKb - availKb) / totalKb) * 100.0;
    stats.usedMb = (totalKb - availKb) / 1024.0;
    stats.totalMb = totalKb / 1024.0;

    return stats;
}

NetworkStats OsMetricsProvider::queryNetworkStats()
{
    NetworkStats networkStats;

    std::ifstream file("/host/proc/net/dev");
    if (!file.is_open())
        return NetworkStats();

    // One time flag to allow data accumulation for comparison
    static bool hasPrevious = false;
    std::string line;

    // Skip the first two header lines
    std::getline(file, line);
    std::getline(file, line);

    uint64_t currentTotalRx = 0;
    uint64_t currentTotalTx = 0;
    uint64_t highestRx = 0;

    while (std::getline(file, line))
    {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream iss(line);

        std::string interface;
        uint64_t rxBytes, rxPackets, rxErrs, rxDrop, rxFifo, rxFrame, rxComp, rxMulti, txBytes;

        // Extract the information
        if (iss >> interface >> rxBytes >> rxPackets >> rxErrs >> rxDrop >> rxFifo >> rxFrame >> rxComp >> rxMulti >> txBytes)
        {
            // Ignore the loopback device and Docker's virtual network bridges
            if (interface == "lo" || interface.find("docker") == 0 || interface.find("br-") == 0 || interface.find("veth") == 0)
            {
                continue;
            }

            currentTotalRx += rxBytes;
            currentTotalTx += txBytes;

            // Because there can be more than one, the active adapter is the one doing the most downloading
            if (rxBytes > highestRx)
            {
                highestRx = rxBytes;
                networkStats.activeAdapter = interface;
            }
        }
    }

    // Calculate the speed between current and last total bytes
    if (hasPrevious)
    {
        // MB/s
        networkStats.download = static_cast<double>(currentTotalRx - mPrevTotalRx);
        networkStats.upload = static_cast<double>(currentTotalTx - mPrevTotalTx);
    }

    mPrevTotalRx = currentTotalRx;
    mPrevTotalTx = currentTotalTx;
    hasPrevious = true;

    return networkStats;
}

std::vector<ProcessInfo> OsMetricsProvider::queryProcessTable()
{
    std::vector<ProcessInfo> infoList;
    infoList.reserve(512);
    
    for (const directory_entry& entry : directory_iterator("/proc"))
    {
        if (!entry.is_directory())
            continue;

        const std::string pidStr = entry.path().filename().string();

        // Check for valid entries
        if (!isAllDigits(pidStr))
            continue;
        
        int pid = -1;
        try
        {
            pid = std::stoi(pidStr);
        }
        catch(...)
        {
            continue;
        }
        
        std::string name;
        const std::string commPath = "/proc/" + pidStr + "/comm";

        if (!readFirstLine(commPath, name))
            continue;

        ProcessInfo info;
        info.pid = pid;
        info.name = name;

        infoList.push_back(std::move(info));
    }

    return infoList;
}

ProcessDetails OsMetricsProvider::queryProcessDetails(int pid)
{
    ProcessDetails details;
    details.pid = pid;

    std::ifstream file("/proc/" + std::to_string(pid) + "/status");
    if (!file.is_open())
        return ProcessDetails();

    std::string line;
    while (std::getline(file, line))
    {
        size_t colonPos = line.find(":");

        if (line.find("Name") != std::string::npos)
        {
            if (colonPos != std::string::npos)
            {
                std::string str = line.substr(colonPos + 1);

                size_t start = str.find_first_not_of(" \n\r\t\f\v");
                if (start != std::string::npos)
                {
                    str.erase(0, start);
                }

                details.name = str;
            }
        }

        if (line.find("Threads") != std::string::npos)
        {
            if (colonPos != std::string::npos)
            {
                std::string str = line.substr(colonPos + 1);

                size_t start = str.find_first_not_of(" \n\r\t\f\v");
                if (start != std::string::npos)
                {
                    str.erase(0, start);
                }

                details.threads = std::stoi(str);
            }
        }

        if (line.find("VmRSS") != std::string::npos)
        {
            if (colonPos != std::string::npos)
            {
                std::string str = line.substr(colonPos + 1);

                size_t start = str.find_first_not_of(" \n\r\t\f\v");
                if (start != std::string::npos)
                {
                    str.erase(0, start);
                }

                // Convert Kb to Mb
                details.memoryRSS = std::stoi(str) / 1024.0;
            }
        }
    }

    return details;
}

bool OsMetricsProvider::isAllDigits(const std::string& str)
{
    if (str.empty())
        return false;

    for (char c : str)
        if (c < '0' || c > '9')
            return false;

    return true;
}

bool OsMetricsProvider::readFirstLine(const std::string &path, std::string &out)
{
    std::ifstream file(path);
    
    if (!file.is_open())
        return false;
    
    std::getline(file, out);
    
    return !out.empty();
}
