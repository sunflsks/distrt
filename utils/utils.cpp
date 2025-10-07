#include <random>

double rand_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

double rand_double(double low, double high) {  // (low, high]
    return low + (high - low) * rand_double();
}