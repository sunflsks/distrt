#pragma once

#include <limits>
#include <numbers>
#include <random>

using limits = std::numeric_limits<double>;

constexpr double deg_to_rad(double deg) {
    return deg * std::numbers::pi / 180;
}

class interval {
   private:
    double l = -std::numeric_limits<double>::infinity();
    double r = std::numeric_limits<double>::infinity();

   public:
    interval(double l, double r) : l(l), r(r) {}

    static interval universe() {
        return interval(-limits::infinity(), limits::infinity());
    }

    static interval vacuous() {
        return interval(limits::infinity(), -limits::infinity());
    }

    static interval universe_positive() {
        return interval(0.001, limits::infinity());
    }

    bool contains_closed(double m) const { return (l <= m) && (m <= r); }

    bool contains_open(double m) const { return (l < m) && (m < r); }

    double upper() const { return r; }
    double lower() const { return l; }

    double clamp(double d) const {
        if (d < l) return l;
        if (d > r) return r;
        return d;
    };
};

double rand_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

double rand_double(double low, double high) {  // (low, high]
    return low + (high - low) * rand_double();
}