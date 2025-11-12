#pragma once

#include <limits>

class Interval {
   private:
    using limits = std::numeric_limits<double>;
    double l = -limits::infinity();
    double r = limits::infinity();

   public:
    Interval(double l, double r) : l(l), r(r) {}

    static Interval universe() { return Interval(-limits::infinity(), limits::infinity()); }

    static Interval vacuous() { return Interval(limits::infinity(), -limits::infinity()); }

    static Interval universe_positive() { return Interval(0.001, limits::infinity()); }

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
