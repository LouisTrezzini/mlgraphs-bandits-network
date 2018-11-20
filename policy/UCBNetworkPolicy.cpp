#include "UCBNetworkPolicy.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

double valueForArmAndUser(unsigned long i, unsigned long u, const Network *network, unsigned long t, matrix<unsigned long> T, matrix<double>X) {
    double S = X(u, i);
    unsigned long m = T(u, i);

    auto neighbors = boost::adjacent_vertices(u, *network);
    for (auto it = neighbors.first; it != neighbors.second; it++) {
        S += X(*it, i);
        m += T(*it, i);
    }

    return S / m + std::sqrt(2 * std::log(t) / m);
}

unsigned long argmaxUCB(unsigned long u, const Network *network, unsigned long t, matrix<unsigned long> T, matrix<double>X) {
    double valueMax = valueForArmAndUser(0, u, network, t, T, X);
    std::vector<unsigned long> indicesMax = {0};

    for (unsigned long i = 1; i < T.size2(); i++) {
        double value = valueForArmAndUser(i, u, network, t, T, X);

        if (value == valueMax) {
            indicesMax.push_back(i);
        }

        if (value > valueMax) {
            valueMax = value;
            indicesMax = {i};
        }
    }

    std::default_random_engine engine;
    std::uniform_int_distribution<unsigned long> dist(0, indicesMax.size() - 1);

    return indicesMax[dist(engine)];
}

bool UCBNetworkPolicy::run(std::default_random_engine &generator, int N) {

    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);

    std::cout << T << std::endl;
    std::cout << X << std::endl;

    for (unsigned long t = 0; t < K; t++) {
        auto vs = boost::vertices(*network);
        for(auto  it = vs.first; it != vs.second; it++) {
            double reward = bandit->getArms()[t]->sample(generator);
            T(*it, t) += 1;
            X(*it, t) += reward;
        }
    }

    for (unsigned long t = K; t < N; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);

        auto vs = boost::vertices(*network);
        for(auto  it = vs.first; it != vs.second; it++) {
            unsigned long selectedArmIdx = argmaxUCB(*it, network, t, T, X);

            double reward = bandit->getArms()[selectedArmIdx]->sample(generator);

            T_next(*it, selectedArmIdx) += 1;
            X_next(*it, selectedArmIdx) += reward;
        }

        T = T_next;
        X = X_next;
    }

    std::cout << T << std::endl;
    std::cout << X << std::endl;

    return true;
}
