#include "sphere.hpp"

#include "utils.hpp"

bool Sphere::hit(const Ray& r, const Interval& t_interval, hit_record& rec) const {
    auto oc = center - r.origin();
    double a = r.direction().dot(r.direction());      // a = (d . d)
    double b_half = r.direction().dot(oc);            // b = 2 * (oc . d)
    double c = oc.dot(oc) - radius * radius;          // c = (oc . oc) - r^2
    double discriminant = (b_half * b_half - a * c);  // b^2 - 4ac, substitute and we get
                                                      // (2b_half)^2 - 4ac, factor out and we get
                                                      // 4(b_half^2 - ac), we can throw away 4 and
                                                      // we get the discriminant (divided by 4)

    if (discriminant < 0) return false;  // no hits

    double d_sqrt = std::sqrt(discriminant);  // sqrt(discriminant / 4) = (1/2)*sqrt(disriminant)
    double root =
        (b_half - d_sqrt) / a;  // (-b - disc) / 2a => 2*b_half - 2(1/2(sqrt(disriminant))) / 2a
                                // => 1/2(sqrt(disriminant)) / a (which is what we have)
                                // don't switch signs of b as it's already negative bc we
                                // r doing things backwards

    if (!t_interval.contains_closed(root)) {  // if x - doesn't work, try x+
        root = (b_half + d_sqrt) / a;
        if (!t_interval.contains_closed(root)) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    rec.mat = mat;
    return true;
}

std::vector<std::byte> Sphere::bytes() const {
    std::vector<std::byte> byte_rep;

    auto identifier = type_to_id[typeid(*this)];
    byte_rep.push_back(identifier);

    // no need to worry about endianness, this code is never touching a big-endian machine.
    auto center_array = center.data();
    constexpr uint64_t pack_size = static_cast<uint64_t>(sizeof(center_array) + sizeof(radius));

    byte_rep.insert(byte_rep.end(),
                    reinterpret_cast<const std::byte*>(&pack_size),
                    reinterpret_cast<const std::byte*>(&pack_size) + sizeof(pack_size));

    byte_rep.insert(
        byte_rep.end(),
        reinterpret_cast<const std::byte*>(center_array.data()),
        reinterpret_cast<const std::byte*>(center_array.data()) + sizeof(center_array.data()));

    // TODO: something w/ mats

    return byte_rep;
}