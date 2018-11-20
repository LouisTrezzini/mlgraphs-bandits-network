#ifndef MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H


#include "IPolicy.h"

class UCBNetworkPolicy : public IPolicy {

public:
    explicit UCBNetworkPolicy(const BanditNetwork *banditNetwork) : IPolicy(banditNetwork) {}

    bool run(std::default_random_engine &generator, int N) override;

};


#endif //MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
