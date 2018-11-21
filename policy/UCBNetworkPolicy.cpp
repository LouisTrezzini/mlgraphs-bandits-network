#include "UCBNetworkPolicy.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

double valueForArmAndUser(
        unsigned long i, unsigned long u, const Network *network, unsigned long t, matrix<unsigned long> T,
        matrix<double> X) {
    double S = X(u, i);
    unsigned long m = T(u, i);

    for (auto neighborIdx: network->getNeighbors(u)) {
        S += X(neighborIdx, i);
        m += T(neighborIdx, i);
    }

    return S / m + std::sqrt(2 * std::log(t) / m);
}

unsigned long UCBNetworkPolicy::argmaxUCB(
        unsigned long u, const Network *network, unsigned long t, matrix<unsigned long> T,
        matrix<double> X) {
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

std::pair<matrix<unsigned long>, matrix<double>> UCBNetworkPolicy::run(std::default_random_engine &generator, int N) {

    auto bandit = this->getBanditNetwork()->getBandit();
    auto network = this->getBanditNetwork()->getNetwork();
    auto K = bandit->getK();

    // FIXME This assumes all vertices are [|0, N - 1|]
    matrix<unsigned long> T = zero_matrix(network->vertex_set().size(), K);
    matrix<double> X = zero_matrix(network->vertex_set().size(), K);

    for (unsigned long t = 0; t < K; t++) {
        for (auto userIdx : network->getVertices()) {
            double reward = bandit->getArms()[t]->sample(generator);
            T(userIdx, t) += 1;
            X(userIdx, t) += reward;
        }
    }

    for (unsigned long t = K; t < N; t++) {
        matrix<unsigned long> T_next(T);
        matrix<double> X_next(X);

        for (auto userIdx : network->getVertices()) {
            unsigned long selectedArmIdx = UCBNetworkPolicy::argmaxUCB(userIdx, network, t, T, X);

            double reward = bandit->getArms()[selectedArmIdx]->sample(generator);

            T_next(userIdx, selectedArmIdx) += 1;
            X_next(userIdx, selectedArmIdx) += reward;
        }

        T = T_next;
        X = X_next;
    }

    return std::pair(T, X);
}
