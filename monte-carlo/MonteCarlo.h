#ifndef MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
#define MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H

#include "../policy/IPolicy.h"

class MonteCarlo {
    int N;
    int horizon;

public:
    MonteCarlo(const int N, const int horizon): N(N), horizon(horizon) {}

    void simulate(IPolicy *policy, const int seed = time(NULL));
};

#endif //MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
