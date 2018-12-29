#ifndef MLGRAPHS_BANDITS_NETWORK_ARMBERNOULLI_H
#define MLGRAPHS_BANDITS_NETWORK_ARMBERNOULLI_H

#include "IArm.h"

class ArmBernoulli : public IArm {
    std::bernoulli_distribution distribution;
public:
    explicit ArmBernoulli(double p) {
        this->distribution = std::bernoulli_distribution(p);
        this->mean = p;
    }

    virtual double sample(RNG &generator) override {
        return distribution(generator);
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_ARMBERNOULLI_H
