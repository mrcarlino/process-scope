#ifndef NETWORK_STATS_H
#define NETWORK_STATS_H

#include <string>

struct NetworkStats
{
    std::string activeAdapter = "--";
    double download = 0.0;
    double upload = 0.0;
};

#endif // NETWORK_STATS_H