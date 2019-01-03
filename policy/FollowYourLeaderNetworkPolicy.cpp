
#include "FollowYourLeaderNetworkPolicy.h"
#include "UCBNetworkPolicy.h"
#include "PolicyResult.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/connected_components.hpp>


using namespace boost::numeric::ublas;


struct UnproperSetOfLeadersException : public std::exception {
    const char *what() const noexcept {
        return "The set of leaders is not compatible with the graph";
    }
};

PolicyResult FollowYourLeaderNetworkPolicy::run(RNG &generator, unsigned long horizon) {
    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);
    std::vector<unsigned long> actions(network->vertex_set().size());
    std::vector<matrix<double>> allRewards;
    std::vector<matrix<unsigned long>> allActions;

    for (unsigned long t = 0; t < horizon; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);
        std::vector<unsigned long> actions_next(actions);

        for (auto userIdx : network->getVertices()) {
            unsigned long selectedArmIdx;

            // It's a leader
            if (leaders.find(userIdx) == leaders.end()) {
                if (t < K) {
                    selectedArmIdx = t;
                } else {
                    selectedArmIdx = UCBNetworkPolicy::argmaxUCB(userIdx, network, t, T, X);
                }
            } else {
                // First round: take a random action
                if (t == 0) {
                    std::uniform_int_distribution<unsigned long> dist(0, K - 1);
                    unsigned long randomArmIdx = dist(generator);

                    selectedArmIdx = randomArmIdx;
                }
                // Follow Your Leader
                else {
                    selectedArmIdx = actions[leaders.at(userIdx)];
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
        allRewards.push_back(X);
        allActions.push_back(T);
    }

    return PolicyResult(allRewards, allActions);
}
