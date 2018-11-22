#ifndef MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
#define MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H


#include <boost/numeric/ublas/matrix.hpp>
#include <vector>

using namespace boost::numeric::ublas;

class PolicyResult {
    std::vector<matrix<double>> allRewards;
    std::vector<matrix<unsigned long>> allActions;
public:
    PolicyResult(const std::vector<matrix<double>> &allRewards, const std::vector<matrix<unsigned long>> &allActions) : allRewards(allRewards), allActions(allActions) {}

    const std::vector<matrix<double>> &getAllRewards() const {
        return allRewards;
    }

    std::vector<double> rewardsOverTime() const {
        std::vector<double> rewards;
        for (int i = 0; i < allRewards.size(); i++) {
            rewards.push_back(PolicyResult::total_reward(allRewards[i]));
        }
        return rewards;
    }

    std::vector<unsigned long> ActionIOverTime(const std::vector<int> &best_arms) const{
        std::vector<unsigned long> action_i_over_time;
        for (int i = 0; i < allRewards.size(); i++) {
            action_i_over_time.push_back(PolicyResult::numberActionI(allActions[i], best_arms));
        }
        return action_i_over_time;
    }


    static double total_reward(const matrix<double> &X) {
        double total = 0;
        for (int i = 0; i < X.size1(); i++) {
            for (int j = 0; j < X.size2(); j++) {
                total += X(i, j);
            }
        }
        return total;
    }

    static double numberActionI(const matrix<unsigned long> &T, std::vector<int> best_arms) {
        double total_actions_i = 0;
        for (int k = 0; k < T.size1(); k++) {
            for (const auto& arm: best_arms) {
                total_actions_i += T(k, arm);
            }

        }
        return total_actions_i;
    }
};

#endif //MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
