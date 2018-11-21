#include "MonteCarlo.h"
#include <time.h>

void MonteCarlo::simulate(IPolicy *policy, const int seed) {
    matrix<unsigned long> T;
    matrix<double> X;
    double total_reward = 0;
    std::default_random_engine generator;
    generator.seed(seed);
    for (int i = 0; i < N; i ++) {
        std::tie(T, X) = policy->run(generator, Time);
        total_reward += IPolicy::total_reward(X);
    }
    total_reward /= N;
    std::cout << "Average final reward: " << total_reward << std::endl;
}