#include "vec3.hpp"

#include <iostream>

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << "x: " << v.x() << ", y: " << v.y() << ", z: " << v.z();
}
