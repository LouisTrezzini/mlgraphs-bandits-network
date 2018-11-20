#pragma once

#include <vector>
#include "../bandit/BanditNetwork.h"


class IPolicy {
    const BanditNetwork *banditNetwork = nullptr;
public:
    explicit IPolicy(const BanditNetwork *banditNetwork) {
        this->banditNetwork = banditNetwork;
    }

    const BanditNetwork* getBanditNetwork() {
        return  this->banditNetwork;
    }

    virtual bool run(std::default_random_engine &generator, int N) = 0;
};
