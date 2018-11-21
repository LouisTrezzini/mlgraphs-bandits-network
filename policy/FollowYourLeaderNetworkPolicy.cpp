
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
        auto vs = boost::vertices(*network);
        for(auto  it = vs.first; it != vs.second; it++) {
            double reward = bandit->getArms()[t]->sample(generator);
            T(*it, t) += 1;
            X(*it, t) += reward;
            previous_actions[*it] = t;
        }
    }

    // FIXME We often loop on leaders which we could avoid
    for (unsigned long t = K; t < N; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);

        auto vs = boost::vertices(*network);
        for(auto  it = vs.first; it != vs.second; it++) {
            unsigned long selectedArmIdx;
            auto search = leaders.find(*it);
            if (search != leaders.end()) {
                selectedArmIdx = UCBNetworkPolicy::argmaxUCB(*it, network, t, T, X);
            }
            else {
                auto neighbours = boost::adjacent_vertices(*it, *network);
                bool found = false;
                for (auto vd : make_iterator_range(neighbours)) {
                    auto search = leaders.find(vd);
                    if (search != leaders.end()) {
                        selectedArmIdx = previous_actions[vd];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    throw UnproperSetOfLeadersException();
                }
            }

            double reward = bandit->getArms()[selectedArmIdx]->sample(generator);

            T_next(*it, selectedArmIdx) += 1;
            X_next(*it, selectedArmIdx) += reward;
            new_actions[*it] = selectedArmIdx;
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
