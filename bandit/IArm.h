#ifndef MLGRAPHS_BANDITS_NETWORK_IARM_H
#define MLGRAPHS_BANDITS_NETWORK_IARM_H


#include <random>

typedef std::default_random_engine RNG;


class IArm {
protected:
    double mean = 0.0;
public:
    double getMean() const {
        return mean;
    }

    virtual double sample(RNG &generator) = 0;
};

#endif //MLGRAPHS_BANDITS_NETWORK_IARM_H
