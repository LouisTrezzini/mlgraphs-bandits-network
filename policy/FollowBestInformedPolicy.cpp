#include "FollowBestInformedPolicy.h"
#include "UCBNetworkPolicy.h"
#include "PolicyResult.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/connected_components.hpp>


using namespace boost::numeric::ublas;

struct InformedNeighbor {
    unsigned long neighborIdx;
    unsigned long numberOfInformation;
};

InformedNeighbor findBestInformedNeighbor(unsigned long u, const Network *network, const vector<unsigned long> & informationAtDisposalForLastAction ) {
    InformedNeighbor bestInformedNeighbor{};
    bestInformedNeighbor.neighborIdx = network->getNeighbors(u)[0];
    bestInformedNeighbor.numberOfInformation = informationAtDisposalForLastAction(bestInformedNeighbor.neighborIdx);

    for (auto neighborIdx: network->getNeighbors(u)) {
        if (informationAtDisposalForLastAction(neighborIdx) > bestInformedNeighbor.numberOfInformation) {
            bestInformedNeighbor.numberOfInformation = informationAtDisposalForLastAction(neighborIdx);
            bestInformedNeighbor.neighborIdx = neighborIdx;
        }
    }

    return bestInformedNeighbor;
}

unsigned long numberOfSamplesAtDisposal(unsigned long u, const Network *network, const matrix<unsigned long> &T) {
    unsigned long numberOfSamples = 0;
    for (auto neighborIdx: network->getNeighbors(u)) {
        for (unsigned long i = 0; i < T.size2(); i++) {
            numberOfSamples += T(u, i);
        }
    }

    return numberOfSamples;
}


PolicyResult FollowBestInformedPolicy::run(RNG &generator, unsigned long horizon) {
    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);
    vector<unsigned long> informationAtDisposalForLastAction(network->vertex_set().size(), 0);
    std::vector<unsigned long> actions(network->vertex_set().size());

    std::vector<matrix<double>> allRewards;
    std::vector<matrix<unsigned long>> allActions;

    for (unsigned long t = 0; t < horizon; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);
        vector<unsigned long> informationAtDisposalForLastAction_next(informationAtDisposalForLastAction);
        std::vector<unsigned long> actions_next(actions);

        for (auto userIdx : network->getVertices()) {
            unsigned long selectedArmIdx;
            InformedNeighbor bestInformedNeighbor = findBestInformedNeighbor(
                    userIdx,
                    network,
                    informationAtDisposalForLastAction
            );
            unsigned long numberOfSamples = numberOfSamplesAtDisposal(userIdx, network, T);

            // It's a leader
            if (bestInformedNeighbor.numberOfInformation <= numberOfSamples) {
                if (t < K) {
                    selectedArmIdx = t;
                } else {
                    selectedArmIdx = UCBNetworkPolicy::argmaxUCB(userIdx, network, t, T, X);
                }
                informationAtDisposalForLastAction_next[userIdx] = numberOfSamples;
            }
            else {
                // First round: take a random action
                if (t == 0) {
                    std::uniform_int_distribution<unsigned long> dist(0, K - 1);
                    unsigned long randomArmIdx = dist(generator);

                    selectedArmIdx = randomArmIdx;
                    informationAtDisposalForLastAction_next[userIdx] = 0;
                }
                // Follow the neighbor with the maximum information
                else {
                    selectedArmIdx = actions[bestInformedNeighbor.neighborIdx];
                    informationAtDisposalForLastAction_next[userIdx] = bestInformedNeighbor.numberOfInformation;
                }
            }

            double reward = bandit->getArms()[selectedArmIdx]->sample(generator);

            T_next(userIdx, selectedArmIdx) += 1;
            X_next(userIdx, selectedArmIdx) += reward;

            actions_next[userIdx] = selectedArmIdx;
        }

        T = T_next;
        X = X_next;
        actions = actions_next;
        informationAtDisposalForLastAction = informationAtDisposalForLastAction_next;
        allRewards.push_back(X);
        allActions.push_back(T);
    }

    return PolicyResult(allRewards, allActions);
}
