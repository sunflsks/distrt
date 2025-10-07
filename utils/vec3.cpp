#include "vec3.hpp"

#include <iostream>

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << "x: " << v.x() << ", y: " << v.y() << ", z: " << v.z();
}
