#ifndef OSMETRICSPROVIDER_H
#define OSMETRICSPROVIDER_H

// STL includes
#include <vector>
#include <stdint.h>

// Local includes
#include "CpuStats.h"
#include "MemoryStats.h"
#include "ProcessInfo.h"
#include "ProcessDetails.h"

class OsMetricsProvider
{
public:
    OsMetricsProvider();
    ~OsMetricsProvider();

    std::string getCpuModelName();

    CpuStats queryCpuStats();
    MemoryStats queryMemoryStats();

    std::vector<ProcessInfo> queryProcessTable();
    ProcessDetails queryProcessDetails(int pid);

private:
    bool isAllDigits(const std::string& str);
    bool readFirstLine(const std::string& path, std::string& out);

    uint64_t mPrevCpuTotal;
    uint64_t mPrevCpuIdle;
};

#endif // OSMETRICSPROVIDER_H
