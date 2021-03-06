#ifndef MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H


#include "IPolicy.h"
#include "PolicyResult.h"

using namespace boost::numeric::ublas;

class UCBNetworkPolicy : public IPolicy {

public:
    explicit UCBNetworkPolicy(const BanditNetwork *banditNetwork) : IPolicy(banditNetwork) {}

    PolicyResult run(RNG &generator, unsigned long horizon) override;

    static unsigned long argmaxUCB(unsigned long u, const Network *network, unsigned long t, const matrix<unsigned long> &T, const matrix<double> &X);

};


#endif //MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
