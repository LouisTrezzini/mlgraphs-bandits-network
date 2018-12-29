#pragma once

#include <vector>
#include "../bandit/BanditNetwork.h"
#include "PolicyResult.h"

class IPolicy {
    const BanditNetwork *banditNetwork = nullptr;
public:
    explicit IPolicy(const BanditNetwork *banditNetwork) {
        this->banditNetwork = banditNetwork;
    }

    const BanditNetwork* getBanditNetwork() {
        return  this->banditNetwork;
    }

    virtual PolicyResult run(RNG &generator, unsigned long horizon) = 0;
};


