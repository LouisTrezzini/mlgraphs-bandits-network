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

    const std::vector<IArm*>& getArms() const {
        return arms;
    }

    unsigned long getK() const {
        return arms.size();
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_BANDIT_H
