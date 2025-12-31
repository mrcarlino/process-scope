#ifndef SYSTEMTOTALS_H
#define SYSTEMTOTALS_H

#include <vector>

struct SystemTotals
{
    double totalCpu     = 0.0;
    double totalMemMb   = 0.0;
    double totalIo      = 0.0;
    int    totalThreads = 0;

    std::vector<double> cpuHistory;
    std::vector<double> memHistory;
};

#endif // SYSTEMTOTALS_H
