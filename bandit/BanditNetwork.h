#include <utility>

#ifndef MLGRAPHS_BANDITS_NETWORK_BANDITNETWORK_H
#define MLGRAPHS_BANDITS_NETWORK_BANDITNETWORK_H

#include "../network/Network.h"
#include "Bandit.h"

class BanditNetwork {
    const Bandit *bandit;
    const Network *network;
public:
    BanditNetwork(const Bandit *bandit, const Network *network) {
        this->bandit = bandit;
        this->network = network;
    }

    const Bandit *getBandit() const {
        return bandit;
    }

    const Network *getNetwork() const {
        return network;
    }
};

#endif //MLGRAPHS_BANDITS_NETWORK_BANDITNETWORK_H
