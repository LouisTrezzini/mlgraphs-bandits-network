#include "MonteCarlo.h"
#include "../policy/PolicyResult.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <future>
#include "../util/ThreadPool.h"

const bool USE_MULTI_THREADING = true;

namespace fs = std::filesystem;

void MonteCarlo::simulate(IPolicy *policy, const std::string &file_name, const int seed) {
    std::vector<double> rewards(horizon, 0);
    std::default_random_engine generator;
    generator.seed(seed);

    auto start = std::chrono::high_resolution_clock::now();

    if(!USE_MULTI_THREADING) {
        for (unsigned long i = 0; i < N; i++) {
            PolicyResult policyResult = policy->run(generator, horizon);
            std::vector<double> rewardsSample = policyResult.rewardsOverTime();
            for (unsigned long t = 0; t < horizon; t++) {
                rewards[t] += rewardsSample[t];
            }
        }
    }
    else {
        std::vector<MultiThreading::ThreadPool::TaskFuture<PolicyResult>> runResults;

        for (unsigned long i = 0; i < N; i++) {
            runResults.push_back(
                    MultiThreading::DefaultThreadPool::submitJob([this, policy, &generator]() -> PolicyResult {
                        return policy->run(generator, horizon);
                    }));
        }

        for (unsigned long i = 0; i < N; i++) {
            std::vector<double> rewardsSample = runResults[i].get().rewardsOverTime();
            for (unsigned long t = 0; t < horizon; t++) {
                rewards[t] += rewardsSample[t];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;

    std::cout << "Monte Carlo simulated " << N << " trajectories in " << elapsedSeconds.count() << "s" << std::endl;

    for (unsigned long t = 0; t < horizon; t++) {
        rewards[t] /= N;
    }

    writeResults(rewards, policy->getBanditNetwork()->maximumRewardPerRound(), file_name);

    std::cout << "Average final reward: " << rewards[rewards.size() - 1] << std::endl;
}

void MonteCarlo::writeResults(std::vector<double> rewards, double maximumRewardPerRound, const std::string &fileName) {
    std::ofstream outFile;
    outFile.open(fileName);
    outFile << "reward maximum_reward" << "\n";
    for (unsigned int round = 0; round < rewards.size(); round++) {
        outFile << rewards[round] << " " << maximumRewardPerRound * (round + 1) << "\n";
    }
    outFile.close();
}