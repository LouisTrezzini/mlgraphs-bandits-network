#include "MonteCarlo.h"
#include "../policy/PolicyResult.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <future>
#include "../util/ThreadPool.h"

namespace fs = std::filesystem;

const bool USE_MULTI_THREADING = true;


void MonteCarlo::simulate(IPolicy *policy, const std::string &file_name, const int seed) {
    std::vector<double> rewards(horizon, 0);
    std::vector<unsigned long> best_actions(horizon, 0);
    std::default_random_engine generator;
    generator.seed(seed);

    auto start = std::chrono::high_resolution_clock::now();

    ArmsAndMean armsAndMean = policy->getBanditNetwork()->maximumRewardPerRoundAndBestArms();
    double maximumRewardPerRound = armsAndMean.mean;
    std::vector<unsigned long> bestArms = armsAndMean.arms;


    if (!USE_MULTI_THREADING) {
        // FIXME Obselete code
        for (unsigned long i = 0; i < N; i++) {
            PolicyResult policyResult = policy->run(generator, horizon);
            std::vector<double> rewardsSample = policyResult.rewardsOverTime();
            for (unsigned long t = 0; t < horizon; t++) {
                rewards[t] += rewardsSample[t];
            }
        }
    } else {
        std::vector<MultiThreading::ThreadPool::TaskFuture<PolicyResult>> runResults;

        for (unsigned long i = 0; i < N; i++) {
            runResults.push_back(
                    MultiThreading::DefaultThreadPool::submitJob([this, policy, &generator]() -> PolicyResult {
                        return policy->run(generator, horizon);
                    }));
        }

        for (unsigned long i = 0; i < N; i++) {
            std::vector<double> rewardsSample = runResults[i].get().rewardsOverTime();
            std::vector<unsigned long> actionsSample = runResults[i].get().ActionIOverTime(bestArms);
            for (unsigned long t = 0; t < horizon; t++) {
                rewards[t] += rewardsSample[t];
                best_actions[t] += actionsSample[t];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;

    std::cout << "Monte Carlo simulated " << N << " trajectories in " << elapsedSeconds.count() << "s" << std::endl;

    for (unsigned long t = 0; t < horizon; t++) {
        rewards[t] /= N;
    }

    writeResults(rewards, best_actions, maximumRewardPerRound, policy->getBanditNetwork()->actionsPerRound(),
                 file_name);

    std::cout << "Average final reward: " << rewards[rewards.size() - 1] << std::endl;
}

void MonteCarlo::writeResults(std::vector<double> rewards, std::vector<unsigned long> best_actions,
                              double maximumRewardPerRound, double actionsPerRound, const std::string &fileName) {
    std::ofstream outFile;
    outFile.open(fileName);
    outFile << "reward maximum_reward best_actions total actions" << "\n";
    for (unsigned int round = 0; round < rewards.size(); round++) {
        outFile << rewards[round] << " " << maximumRewardPerRound * (round + 1) << " " << best_actions[round] << " "
                << actionsPerRound * (round + 1) << "\n";
    }
    outFile.close();
}