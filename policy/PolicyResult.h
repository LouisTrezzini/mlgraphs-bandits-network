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
        for (const auto &allReward : allRewards) {
            rewards.push_back(PolicyResult::totalReward(allReward));
        }
        return rewards;
    }

    std::vector<unsigned long> ActionIOverTime(const std::vector<unsigned long> &bestArms) const{
        std::vector<unsigned long> actionIOverTime;
        for (int i = 0; i < allRewards.size(); i++) {
            actionIOverTime.push_back(PolicyResult::numberActionI(allActions[i], bestArms));
        }
        return actionIOverTime;
    }


    static double totalReward(const matrix<double> &X) {
        double total = 0;
        for (unsigned long i = 0; i < X.size1(); i++) {
            for (unsigned long j = 0; j < X.size2(); j++) {
                total += X(i, j);
            }
        }
        return total;
    }

    static unsigned long numberActionI(const matrix<unsigned long> &T, std::vector<unsigned long> bestArms) {
        unsigned long totalActionsI = 0;
        for (unsigned long k = 0; k < T.size1(); k++) {
            for (const auto& arm: bestArms) {
                totalActionsI += T(k, arm);
            }

        }
        return totalActionsI;
    }
};

#endif //MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
