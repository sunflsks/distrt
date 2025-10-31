#pragma once

#include "utils.hpp"
#include "vec3.hpp"

class Ray {
   public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    const Vec3& origin() const { return orig; }
    const Vec3& direction() const { return dir; }

    Vec3 at(double t) const { return orig + dir * t; }

    static Vec3 rand() { return Vec3(rand_double(), rand_double(), rand_double()); }

    static Vec3 rand(double min, double max) {
        return Vec3(rand_double(min, max), rand_double(min, max), rand_double(min, max));
    }

    static Vec3 rand_unit_vec() {
        while (true) {
            Vec3 vec = rand(-1, 1);

            // length = sqrt(x^2 + y^2 + z^2)
            // length^2 = x^2 + y^2 + z^2
            // if lenght^2 <= x^2 + y^2 + z^2 then we r inside
            // if length^2 <= 1 then we r inside

            if ((vec.length() * vec.length()) <= 1) return vec.unit_vector();
        }
    }

    // returns a vector that is "aligned" with the normal vector passed in below
    // (positive dot)
    static Vec3 rand_on_hemi(const Vec3& norm) {
        Vec3 unit_vec = rand_unit_vec();
        if (unit_vec.dot(norm) < 0) {
            return -unit_vec;
        }

        return unit_vec;
    }

   private:
    Vec3 orig;
    Vec3 dir;
};