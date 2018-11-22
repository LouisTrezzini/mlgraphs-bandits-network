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

    std::vector<double> rewardsOverTime() const {
        std::vector<double> rewards;
        for (const auto &stepRewards : allRewards) {
            rewards.push_back(PolicyResult::totalReward(stepRewards));
        }
        return rewards;
    }

    std::vector<unsigned long> actionOverTime(const std::vector<unsigned long> &bestArms) const{
        std::vector<unsigned long> actionOverTime;
        for (const auto &stepActions : allActions) {
            actionOverTime.push_back(PolicyResult::countAction(stepActions, bestArms));
        }
        return actionOverTime;
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

    static unsigned long countAction(const matrix<unsigned long> &T, std::vector<unsigned long> bestArms) {
        unsigned long count = 0;
        for (unsigned long k = 0; k < T.size1(); k++) {
            for (unsigned long arm: bestArms) {
                count += T(k, arm);
            }
        }
        return count;
    }
};

#endif //MLGRAPHS_BANDITS_NETWORK_POLICYRESULT_H
