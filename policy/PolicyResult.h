#ifndef MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
#define MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H


#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

class PolicyResult {
    std::vector<matrix<double>> allRewards;
public:
    PolicyResult(const std::vector<matrix<double>> &allRewards) : allRewards(allRewards) {}

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


    static double total_reward(const matrix<double> &X) {
        double total = 0;
        for (int i = 0; i < X.size1(); i++) {
            for (int j = 0; j < X.size2(); j++) {
                total += X(i, j);
            }
        }
        return total;
    }
};

#endif //MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
