#ifndef MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
#define MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H


#include "IPolicy.h"
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

class UCBNetworkPolicy : public IPolicy {

public:
    explicit UCBNetworkPolicy(const BanditNetwork *banditNetwork) : IPolicy(banditNetwork) {}

    std::pair<matrix<unsigned long>, matrix<double>>  run(std::default_random_engine &generator, unsigned long horizon) override;

    static unsigned long argmaxUCB(unsigned long u, const Network *network, unsigned long t, matrix<unsigned long> T, matrix<double>X);

};


#endif //MLGRAPHS_BANDITS_NETWORK_UCBNETWORKPOLICY_H
