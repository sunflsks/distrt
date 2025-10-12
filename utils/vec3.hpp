#pragma once

#include <cmath>
#include <iostream>

// make the following all inline for header only usage

class vec3 {
   private:
    double e[3];

   public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double z() const { return e[2]; }

    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline double operator[](int i) const { return e[i]; }
    inline double &operator[](int i) { return e[i]; }
    inline vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    inline vec3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3 &operator/=(const double t) { return *this *= 1 / t; }

    inline vec3 operator+(const vec3 &v) const {
        return vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
    }

    inline vec3 operator-(const vec3 &v) const {
        return vec3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
    }

    inline vec3 operator*(double x) const { return vec3(e[0] * x, e[1] * x, e[2] * x); }

    inline vec3 operator*(const vec3 &v) const {
        return vec3(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]);
    }

    inline vec3 operator/(double t) const { return *this * (1 / t); }

    inline double length() const { return std::sqrt(length_squared()); }

    inline double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline double dot(const vec3 &v) const { return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2]; }

    inline vec3 cross(const vec3 &v) const {
        return vec3(e[1] * v.e[2] - e[2] * v.e[1], e[2] * v.e[0] - e[0] * v.e[2],
                    e[0] * v.e[1] - e[1] * v.e[0]);
    }

    constexpr inline vec3 gamma_transform() const {
        double e_new[3] = {0.0};

        for (int i = 0; i < 3; i++) {
            e_new[i] = e[i] > 0 ? std::sqrt(e[i]) : 0;
        }

        return vec3(e_new[0], e_new[1], e_new[2]);
    }

    inline vec3 unit_vector() const { return *this / this->length(); }
};

inline vec3 operator*(double t, const vec3 &v) {
    return v * t;
}

using point3 = vec3;
using color = vec3;