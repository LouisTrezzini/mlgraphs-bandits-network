#include <iostream>
#include <cstdlib>
#include "network/NetworkFactory.h"
#include "network/Network.h"
#include "network/Edge.h"
#include "bandit/Bandit.h"
#include "bandit/BanditNetwork.h"
#include "bandit/ArmBernoulli.h"
#include "policy/UCBNetworkPolicy.h"
#include "policy/FollowYourLeaderPolicy.h"
#include "policy/FollowBestInformedPolicy.h"
#include "util/GraphViz.h"
#include "monte-carlo/MonteCarlo.h"


int main(int argc, char *argv[]) {

    int mode = 4;
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

            RNG generator;
            policy.run(generator, 10000);

            return 0;
        }

        // Experiment 1
        case 1: {
            Network star = NetworkFactory::createStarGraph(10);
            Network fc = NetworkFactory::createFullyConnectedGraph(10);
            Network fd = NetworkFactory::createFullyDisconnectedGraph(10);
            Network circular = NetworkFactory::createCircularGraph(10);


            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.7),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetworkStar(&MAB, &star);
            const BanditNetwork banditNetworkFC(&MAB, &fc);
            const BanditNetwork banditNetworkFD(&MAB, &fd);
            const BanditNetwork banditNetworkCircular(&MAB, &circular);

            IPolicy *policyStar = new UCBNetworkPolicy(&banditNetworkStar);
            IPolicy *policyFC = new UCBNetworkPolicy(&banditNetworkFC);
            IPolicy *policyFD = new UCBNetworkPolicy(&banditNetworkFD);
            IPolicy *policyCircular = new UCBNetworkPolicy(&banditNetworkCircular);

            MonteCarlo monteCarloSimulator(1000, 100000);

            std::cout << "UCB policy star network" << std::endl;
            monteCarloSimulator.simulate(policyStar, "../experiment-1/UCBStar.txt");
            std::cout << "UCB policy FC network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyFC, "../experiment-1/UCBFC.txt");
            std::cout << "UCB policy FD network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyFD, "../experiment-1/UCBFD.txt");
            std::cout << "UCB policy circular network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyCircular, "../experiment-1/UCBCircular.txt");

            return 0;
        }

        // Experiment 2
        case 2: {
            Network star = NetworkFactory::createStarGraph(20);
            Network fc = NetworkFactory::createFullyConnectedGraph(20);
            Network fd = NetworkFactory::createFullyDisconnectedGraph(20);
            Network circular = NetworkFactory::createCircularGraph(20);


            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.1),
                    new ArmBernoulli(0.2),
                    new ArmBernoulli(0.3),
                    new ArmBernoulli(0.4),
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.6),
                    new ArmBernoulli(0.7),
                    new ArmBernoulli(0.8),
                    new ArmBernoulli(0.9),
                    new ArmBernoulli(1.0),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetworkStar(&MAB, &star);
            const BanditNetwork banditNetworkFC(&MAB, &fc);
            const BanditNetwork banditNetworkFD(&MAB, &fd);
            const BanditNetwork banditNetworkCircular(&MAB, &circular);


            IPolicy *policyStar = new UCBNetworkPolicy(&banditNetworkStar);
            IPolicy *policyFC = new UCBNetworkPolicy(&banditNetworkFC);
            IPolicy *policyFD = new UCBNetworkPolicy(&banditNetworkFD);
            IPolicy *policyCircular = new UCBNetworkPolicy(&banditNetworkCircular);

            MonteCarlo monteCarloSimulator(1000, 100000);

            std::cout << "UCB policy star network" << std::endl;
            monteCarloSimulator.simulate(policyStar, "../experiment-2/UCBStar.txt");
            std::cout << "UCB policy FC network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyFC, "../experiment-2/UCBFC.txt");
            std::cout << "UCB policy FD network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyFD, "../experiment-2/UCBFD.txt");
            std::cout << "UCB policy circular network" << std::endl;
            // No seed is specified, so the random seed is used
            monteCarloSimulator.simulate(policyCircular, "../experiment-2/UCBCircular.txt");

            return 0;
        }

        // Experiment 3
        case 3: {
            Network star1 = NetworkFactory::createStarGraph(25);
            Network star2 = NetworkFactory::createStarGraph(100);
            Network star3 = NetworkFactory::createStarGraph(350);

            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.7),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetwork25(&MAB, &star1);
            const BanditNetwork banditNetwork100(&MAB, &star2);
            const BanditNetwork banditNetwork350(&MAB, &star3);

            auto leaders25 = NetworkFactory::createStarGraphLeaders(25);
            IPolicy *policyFYL25 = new FollowYourLeaderPolicy(&banditNetwork25, leaders25);
            IPolicy *policyUCB25 = new UCBNetworkPolicy(&banditNetwork25);

            auto leaders100 = NetworkFactory::createStarGraphLeaders(100);
            IPolicy *policyFYL100 = new FollowYourLeaderPolicy(&banditNetwork100, leaders100);
            IPolicy *policyUCB100 = new UCBNetworkPolicy(&banditNetwork100);

            auto leaders350 = NetworkFactory::createStarGraphLeaders(350);
            IPolicy *policyFYL350 = new FollowYourLeaderPolicy(&banditNetwork350, leaders350);
            IPolicy *policyUCB350 = new UCBNetworkPolicy(&banditNetwork350);

            MonteCarlo monteCarloSimulator(1000, 100000);

            std::cout << "Follow your leader policy, star graph with 25 nodes" << std::endl;
            monteCarloSimulator.simulate(policyFYL25, "../experiment-3/FYL25.txt", 0);
            std::cout << "UCB policy, star graph with 25 nodes" << std::endl;
            monteCarloSimulator.simulate(policyUCB25, "../experiment-3/UCB25.txt", 0);

            std::cout << "Follow your leader policy, star graph with 100 nodes" << std::endl;
            monteCarloSimulator.simulate(policyFYL100, "../experiment-3/FYL100.txt", 0);
            std::cout << "UCB policy, star graph with 100 nodes" << std::endl;
            monteCarloSimulator.simulate(policyUCB100, "../experiment-3/UCB100.txt", 0);

            std::cout << "Follow your leader policy, star graph with 350 nodes" << std::endl;
            monteCarloSimulator.simulate(policyFYL350, "../experiment-3/FYL350.txt", 0);
            std::cout << "UCB, star graph with 350 nodes" << std::endl;
            monteCarloSimulator.simulate(policyUCB350, "../experiment-3/UCB350.txt", 0);

            return 0;
        }

        // Experiment 4
        case 4: {
            std::vector<IArm *> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.7),
            };
            const Bandit MAB(arms);

            unsigned long numberOfStars = 5;
            std::vector<unsigned long> numberOfChildren{12, 12, 12, 12, 48};
            Network fcStars = NetworkFactory::createFullyConnectedStarsGraph(numberOfStars, numberOfChildren);
            auto fcStarsLeaders = NetworkFactory::createFullyConnectedStarsGraphLeaders(numberOfStars, numberOfChildren);
            GraphViz::write(fcStars, "network.graphviz");

            const BanditNetwork fcStarsBanditNetwork(&MAB, &fcStars);

            IPolicy *policyFYL = new FollowYourLeaderPolicy(&fcStarsBanditNetwork, fcStarsLeaders);
            IPolicy *policyFBI = new FollowBestInformedPolicy(&fcStarsBanditNetwork);
            IPolicy *policyUCB = new UCBNetworkPolicy(&fcStarsBanditNetwork);

            MonteCarlo monteCarloSimulator(1000, 100000);

            std::cout << "Follow best informed policy, 5 stars graph with fully connected leaders" << std::endl;
            monteCarloSimulator.simulate(policyFBI, "../experiment-4/FBI.txt", 0);
            std::cout << "Follow your leader policy, 5 stars graph with fully connected leaders" << std::endl;
            monteCarloSimulator.simulate(policyFYL, "../experiment-4/FYL.txt", 0);
            std::cout << "UCB-Network policy, 5 stars graph with fully connected leaders" << std::endl;
            monteCarloSimulator.simulate(policyUCB, "../experiment-4/UCB.txt", 0);

            const Network star = NetworkFactory::createStarGraph(100);
            auto starLeaders = NetworkFactory::createStarGraphLeaders(100);

            const BanditNetwork starBanditNetwork(&MAB, &star);

            policyFYL = new FollowYourLeaderPolicy(&starBanditNetwork, starLeaders);
            policyFBI = new FollowBestInformedPolicy(&starBanditNetwork);
            policyUCB = new UCBNetworkPolicy(&starBanditNetwork);

            std::cout << "Follow best informed policy, 100-nodes star graph" << std::endl;
            monteCarloSimulator.simulate(policyFBI, "../experiment-4/FBI-star.txt", 0);
            std::cout << "Follow your leader policy, 100-nodes star graph" << std::endl;
            monteCarloSimulator.simulate(policyFYL, "../experiment-4/FYL-star.txt", 0);
            std::cout << "UCB-Network policy, 100-nodes star graph" << std::endl;
            monteCarloSimulator.simulate(policyUCB, "../experiment-4/UCB-star.txt", 0);


            return 0;
        }

        default:
            return 0;
    }
}
