#pragma once

#include <cmath>
#include <iostream>

class vec3 {
   private:
    double e[3];

   public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }
    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    vec3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t) { return *this *= 1 / t; }

    vec3 operator+(const vec3 &v) const {
        return vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
    }

    vec3 operator-(const vec3 &v) const {
        return vec3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
    }

    vec3 operator*(double x) const {
        return vec3(e[0] * x, e[1] * x, e[2] * x);
    }

    vec3 operator/(double t) const { return *this * (1 / t); }

    double length() const { return std::sqrt(length_squared()); }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double dot(const vec3 &v) const {
        return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2];
    }

    double cross(const vec3 &v) const {
        return e[0] * v.e[1] - e[1] * v.e[0] + e[2] * v.e[0] - e[0] * v.e[2] +
               e[1] * v.e[2] - e[2] * v.e[1];
    }

    vec3 unit_vector() const { return *this / this->length(); }
};

vec3 operator*(double t, const vec3 &v) {
    return v * t;
}

using point3 = vec3;
using color = vec3;

std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << "x: " << v.x() << ", y: " << v.y() << ", z: " << v.z();
}
