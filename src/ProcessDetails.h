#ifndef PROCESSDETAILS_H
#define PROCESSDETAILS_H

#include <string>
#include <vector>

struct ProcessDetails
{
    std::string name;
    int         pid     = -1;
    int         threads = 0;
    double      cpu     = 0.0;
    double      meMb    = 0.0;
    double      io      = 0.0;

    std::vector<double> cpuHistory;
    std::vector<double> memHistory;
};

#endif // PROCESSDETAILS_H
