
#include "FollowYourLeaderNetworkPolicy.h"
#include "UCBNetworkPolicy.h"
#include "PolicyResult.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/connected_components.hpp>


using namespace boost::numeric::ublas;


struct UnproperSetOfLeadersException : public std::exception
{
    const char * what () const throw ()
    {
        return "The set of leaders is not compatible with the graph";
    }
};

PolicyResult FollowYourLeaderNetworkPolicy::run(
        std::default_random_engine &generator, unsigned long horizon) {

    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);
    std::vector<unsigned long> actions(network->vertex_set().size());
    std::vector<matrix<double>> all_rewards;


    // FIXME We could make a search on neighbours and leaders only once
    for (unsigned long t = 0; t < horizon; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);
        std::vector<unsigned long> actions_next(actions);

        for (auto userIdx : network->getVertices()) {
            unsigned long selectedArmIdx;

            if (leaders.find(userIdx) != leaders.end()) {
                if (t < K) {
                    selectedArmIdx = t;
                }
                else {
                    selectedArmIdx = UCBNetworkPolicy::argmaxUCB(userIdx, network, t, T, X);
                }
            }
            else {
                if (t == 0) {
                    std::uniform_int_distribution<unsigned long> dist(0, K - 1);
                    unsigned long randomArmIdx = dist(generator);

                    selectedArmIdx = randomArmIdx;
                }
                else {
                    bool found = false;
                    for (auto neighborIdx : network->getNeighbors(userIdx)) {
                        if (leaders.find(neighborIdx) != leaders.end()) {
                            selectedArmIdx = actions[neighborIdx];
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        throw UnproperSetOfLeadersException();
                    }
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
        all_rewards.push_back(X);
    }

    return PolicyResult(all_rewards);
}
