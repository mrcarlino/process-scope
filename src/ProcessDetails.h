#ifndef PROCESSDETAILS_H
#define PROCESSDETAILS_H

#include <string>
#include <vector>

struct ProcessDetails
{
    std::string name;
    int pid = -1;
    int threads = -1;
};

#endif // PROCESSDETAILS_H
