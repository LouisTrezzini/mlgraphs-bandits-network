#include "MonteCarlo.h"
#include "../policy/PolicyResult.h"
#include <chrono>

void MonteCarlo::simulate(IPolicy *policy, const int seed) {
    std::vector<double> rewards(horizon, 0);
    std::default_random_engine generator;
    generator.seed(seed);

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned long i = 0; i < N; i ++) {
        PolicyResult policy_result = policy->run(generator, horizon);
        std::vector<double> rewards_sample = policy_result.rewardsOverTime();
        for (unsigned long t = 0; t < horizon; t++) {
            rewards[t] += rewards_sample[t];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;

    std::cout << "Monte Carlo simulated " << N << " trajectories in " << elapsedSeconds.count() << "s" << std::endl;

    for (unsigned long t = 0; t < horizon; t++) {
        rewards[t] /= N;
    }
    std::cout << "Average final reward: " << rewards[rewards.size() - 1] << std::endl;
}