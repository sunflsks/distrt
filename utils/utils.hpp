#pragma once

#include <limits>
#include <numbers>

using limits = std::numeric_limits<double>;

class interval {
   private:
    double l = -std::numeric_limits<double>::infinity();
    double r = std::numeric_limits<double>::infinity();

   public:
    interval(double l, double r) : l(l), r(r) {}

    static interval universe() { return interval(-limits::infinity(), limits::infinity()); }

    static interval vacuous() { return interval(limits::infinity(), -limits::infinity()); }

    static interval universe_positive() { return interval(0.001, limits::infinity()); }

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

double rand_double();
double rand_double(double low, double high);  // (low, high]

constexpr double DEG_TO_RAD = std::numbers::pi / 180;
