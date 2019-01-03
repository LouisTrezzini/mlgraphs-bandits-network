#ifndef MLGRAPHS_BANDITS_NETWORK_FOLLOWBESTINFORMEDPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_FOLLOWBESTINFORMEDPOLICY_H

#include "IPolicy.h"

class FollowBestInformedPolicy: public IPolicy {

public:
    explicit FollowBestInformedPolicy(const BanditNetwork *banditNetwork) : IPolicy(banditNetwork) {}

    PolicyResult run(RNG &generator, unsigned long horizon) override;

};

#endif //MLGRAPHS_BANDITS_NETWORK_FOLLOWBESTINFORMEDPOLICY_H
