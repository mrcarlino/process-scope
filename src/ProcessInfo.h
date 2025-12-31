#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

struct ProcessInfo
{
    std::string name;
    int         pid        = -1;
    double      cpuPercent = 0.0;
    double      memMb      = 0.0;
};

#endif // PROCESSINFO_H
