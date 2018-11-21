#ifndef MLGRAPHS_BANDITS_NETWORK_BANDIT_H
#define MLGRAPHS_BANDITS_NETWORK_BANDIT_H

#include <vector>
#include "IArm.h"

class Bandit {
    std::vector<IArm*> arms;
public:
    explicit Bandit(const std::vector<IArm*>& arms) {
        this->arms = arms;
    }

    double getBestMean() const {
        double bestMean = -std::numeric_limits<double>::infinity();
        for (unsigned long i = 0; i < arms.size(); i ++) {
            bestMean = std::max(arms[i]->getMean(), bestMean);
        }
        return bestMean;
    }

    const std::vector<IArm*>& getArms() const {
        return arms;
    }

    unsigned long getK() const {
        return arms.size();
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_BANDIT_H
