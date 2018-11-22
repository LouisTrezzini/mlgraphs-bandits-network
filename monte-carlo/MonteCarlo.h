#ifndef MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
#define MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H

#include "../policy/IPolicy.h"

class MonteCarlo {
    int N;
    int horizon;

public:
    MonteCarlo(const int N, const int horizon): N(N), horizon(horizon) {}

    void simulate(IPolicy *policy, const std::string &file_name, const int seed = time(NULL));

    static void writeResults(std::vector<double> rewards, std::vector<unsigned long> best_actions, double maximumRewardPerRound, double actionsPerRound, const std::string &fileName);
};

#endif //MLGRAPHS_BANDITS_NETWORK_MONTECARLO_H
