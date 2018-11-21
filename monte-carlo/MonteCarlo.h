#ifndef MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
#define MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H

#include "../policy/IPolicy.h"

class MonteCarlo {
    int N;
    int Time;

public:
    MonteCarlo(const int N, const int Time): N(N), Time(Time) {}

    void simulate(IPolicy *policy, const int seed = time(NULL));
};

#endif //MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
