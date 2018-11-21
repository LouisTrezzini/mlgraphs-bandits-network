#include <iostream>
#include <cstdlib>
#include "network/NetworkFactory.h"
#include "network/Network.h"
#include "network/Edge.h"
#include "bandit/Bandit.h"
#include "bandit/BanditNetwork.h"
#include "bandit/ArmBernoulli.h"
#include "policy/UCBNetworkPolicy.h"
#include "policy/FollowYourLeaderNetworkPolicy.h"
#include "util/GraphViz.h"
#include "monte-carlo/MonteCarlo.h"


int main(int argc, char *argv[]) {

    int mode = 1;
    if (argc >= 2) mode = atoi(argv[1]);
    switch (mode) {

        case 0: {
            Network g = NetworkFactory::createStarGraph(5);

            GraphViz::write(g, "network.graphviz");

            std::cout << g.vertex_set() << std::endl;

            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.7),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetwork(&MAB, &g);

            UCBNetworkPolicy policy(&banditNetwork);

            std::default_random_engine generator;
            policy.run(generator, 10000);

            return 0;
        }

        case 1: {
            Network g = NetworkFactory::createStarGraph(5);

            GraphViz::write(g, "network.graphviz");

            std::cout << g.vertex_set() << std::endl;

            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.7),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetwork(&MAB, &g);

            const std::unordered_set<int> leaders = {0};
            IPolicy *policy1 = new FollowYourLeaderNetworkPolicy(&banditNetwork, leaders);
            IPolicy *policy2 = new UCBNetworkPolicy(&banditNetwork);

            MonteCarlo monte_carlo_simulator(100, 10000);

            std::cout << "Follow your leader policy" << std::endl;
            // I specify the seed 0
            monte_carlo_simulator.simulate(policy1, 0);
            std::cout << "UCB policy" << std::endl;
            // No seed is specified, so the random seed is used
            monte_carlo_simulator.simulate(policy2);

            return 0;
        }

        default:
            return 0;
    }
}
