
#include <cmath>
#include "Statistics.h"

double mean(std::vector<double> vec) {
    double result = 0;
    for (double i : vec) {
        result += i;
    }
    return result / vec.size();
}


double standardDeviation(std::vector<double> vec) {
    double meanValue = mean(vec);
    double result = 0;
    for (double i : vec) {
        result += std::pow(i - meanValue, 2);
    }
    return std::sqrt(result / vec.size());
}




