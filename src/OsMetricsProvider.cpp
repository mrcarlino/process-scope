#include "OsMetricsProvider.h"

OsMetricsProvider::OsMetricsProvider()
{
}

OsMetricsProvider::~OsMetricsProvider()
{
}

std::vector<ProcessInfo> OsMetricsProvider::queryProcessTable()
{
    return std::vector<ProcessInfo>();
}

SystemTotals OsMetricsProvider::querySystemTotals()
{
    return SystemTotals();
}

ProcessDetails OsMetricsProvider::queryProcessDetails(int pid)
{
    return ProcessDetails();
}
