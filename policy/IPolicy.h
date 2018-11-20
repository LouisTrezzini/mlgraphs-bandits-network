#pragma once

#include <vector>
#include "../bandit/BanditNetwork.h"
#include <boost/numeric/ublas/matrix.hpp>

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

    virtual bool run(std::default_random_engine &generator, int N) = 0;

    static double total_reward(const matrix<double> &X) {
        double total = 0;
        for (int i = 0; i < X.size1(); i++) {
            for (int j = 0; j < X.size2(); j++) {
                total += X(i, j);
            }
        }
        return total;
    }
};
