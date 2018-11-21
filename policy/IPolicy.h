#pragma once

#include <vector>
#include "../bandit/BanditNetwork.h"
#include <boost/numeric/ublas/matrix.hpp>
#include "PolicyResult.h"

using namespace boost::numeric::ublas;

class IPolicy {
    const BanditNetwork *banditNetwork = nullptr;
public:
    explicit IPolicy(const BanditNetwork *banditNetwork) {
        this->banditNetwork = banditNetwork;
    }

    const BanditNetwork* getBanditNetwork() {
        return  this->banditNetwork;
    }

    virtual PolicyResult run(std::default_random_engine &generator,
                             unsigned long horizon) = 0;
};


