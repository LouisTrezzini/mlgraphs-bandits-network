#include "MonteCarlo.h"
#include <chrono>

void MonteCarlo::simulate(IPolicy *policy, const int seed) {
    matrix<unsigned long> T;
    matrix<double> X;
    double total_reward = 0;
    std::default_random_engine generator;
    generator.seed(seed);

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned long i = 0; i < N; i ++) {
        std::tie(T, X) = policy->run(generator, horizon);
        total_reward += IPolicy::total_reward(X);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;

    std::cout << "Monte Carlo simulated " << N << " trajectories in " << elapsedSeconds.count() << "s" << std::endl;

    total_reward /= N;
    std::cout << "Average final reward: " << total_reward << std::endl;
}