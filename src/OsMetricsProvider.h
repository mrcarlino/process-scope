#ifndef OSMETRICSPROVIDER_H
#define OSMETRICSPROVIDER_H

// STL includes
#include <vector>

// Local includes
#include "ProcessInfo.h"
#include "SystemTotals.h"
#include "ProcessDetails.h"

class OsMetricsProvider
{
public:
    OsMetricsProvider();
    ~OsMetricsProvider();

    std::vector<ProcessInfo> queryProcessTable();
    SystemTotals querySystemTotals();
    ProcessDetails queryProcessDetails(int pid);

private:

};

#endif // OSMETRICSPROVIDER_H
