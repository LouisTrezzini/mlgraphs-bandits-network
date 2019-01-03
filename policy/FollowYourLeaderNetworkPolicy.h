#ifndef MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H

#include "IPolicy.h"

class FollowYourLeaderNetworkPolicy : public IPolicy {
    const std::unordered_map<unsigned long, unsigned long> leaders;

public:
    explicit FollowYourLeaderNetworkPolicy(const BanditNetwork *banditNetwork, const std::unordered_map<unsigned long, unsigned long> &leaders) : IPolicy(banditNetwork), leaders(leaders) {}

    PolicyResult run(RNG &generator, unsigned long horizon) override;

};

#endif //MLGRAPHS_BANDITS_NETWORK_FOLLOWYOURLEADERNETWORKPOLICY_H
