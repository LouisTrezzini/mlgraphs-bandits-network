#ifndef MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H

#include "IPolicy.h"

class FollowYourLeaderNetworkPolicy : public IPolicy {
    const std::unordered_set<int> leaders;

public:
    explicit FollowYourLeaderNetworkPolicy(const BanditNetwork *banditNetwork, const std::unordered_set<int> &leaders) : IPolicy(banditNetwork), leaders(leaders) {}

    PolicyResult run(std::default_random_engine &generator, unsigned long horizon) override;

};

#endif //MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H
