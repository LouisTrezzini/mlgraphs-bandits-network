#ifndef MLGRAPHS_BANDITS_NETWORK_BANDIT_H
#define MLGRAPHS_BANDITS_NETWORK_BANDIT_H

#include <vector>
#include "IArm.h"

struct ArmsAndMean{
    double mean;
    std::vector<int> arms;
};

class Bandit {
    std::vector<IArm*> arms;
public:
    explicit Bandit(const std::vector<IArm*>& arms) {
        this->arms = arms;
    }

    ArmsAndMean getBestArmAndMean() const {
        double bestMean = -std::numeric_limits<double>::infinity();
        std::vector<int> best_arms;
        for (unsigned long i = 0; i < arms.size(); i ++) {
            if (bestMean == arms[i]->getMean() {
                arms.push_back(i);
            }
            if (bestMean < arms[i]->getMean()) {
                bestMean = arms[i]->getMean();
                best_arms = std::vector<int> {i};
            }

        }
        return ArmsAndMean {bestMean, best_arms};
    }

    const std::vector<IArm*>& getArms() const {
        return arms;
    }

    unsigned long getK() const {
        return arms.size();
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_BANDIT_H
