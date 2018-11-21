
#include "FollowYourLeaderNetworkPolicy.h"
#include "UCBNetworkPolicy.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>


using namespace boost::numeric::ublas;


struct UnproperSetOfLeadersException : public std::exception
{
    const char * what () const throw ()
    {
        return "The set of leaders is not compatible with the graph";
    }
};

bool FollowYourLeaderNetworkPolicy::run(std::default_random_engine &generator, int N) {

    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);
    std::vector<int> previous_actions(network->vertex_set().size());
    std::vector<int> new_actions(network->vertex_set().size());

    for (unsigned long t = 0; t < K; t++) {
        for (auto userIdx : network->getVertices()) {
            double reward = bandit->getArms()[t]->sample(generator);
            T(userIdx, t) += 1;
            X(userIdx, t) += reward;
            previous_actions[userIdx] = t;
        }
    }

    // FIXME We could make a search on neighbours and leaders only once
    for (unsigned long t = K; t < N; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);

        for (auto userIdx : network->getVertices()) {
            unsigned long selectedArmIdx;

            if (leaders.find(userIdx) != leaders.end()) {
                selectedArmIdx = UCBNetworkPolicy::argmaxUCB(userIdx, network, t, T, X);
            }
            else {
                bool found = false;
                for (auto neighborIdx : network->getNeighbors(userIdx)) {
                    if (leaders.find(neighborIdx) != leaders.end()) {
                        selectedArmIdx = previous_actions[neighborIdx];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    throw UnproperSetOfLeadersException();
                }
            }

            double reward = bandit->getArms()[selectedArmIdx]->sample(generator);

            T_next(userIdx, selectedArmIdx) += 1;
            X_next(userIdx, selectedArmIdx) += reward;
            new_actions[userIdx] = selectedArmIdx;
        }

        previous_actions = new_actions;
        T = T_next;
        X = X_next;
    }

    std::cout << T << std::endl;
    std::cout << X << std::endl;
    std::cout << "Total reward " << IPolicy::total_reward(X);

    return true;
}
