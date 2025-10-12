#include "camera.hpp"

#include "material.hpp"

color camera::ray_color(const ray& r, const hittable& tgt, int max) {
    hittable::hit_record rec;
    if (tgt.hit(r, interval::universe_positive(), rec) && max < 20) {
        auto scattered_ray = rec.mat->scatter(r, rec);
        if (scattered_ray) {
            return scattered_ray->attenuation * ray_color(scattered_ray->scattered, tgt, max + 1);
        }
    }

    // sky
    double a = 0.5 * (r.direction().unit_vector().y() + 1);
    return (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
}

void camera::refresh() {
    height = static_cast<int>(width / aspect_ratio);

    focal_length = (center - target).length();  // how far from the cam to the tgt?

    viewport_height = 2 * (std::tan((vfov * DEG_TO_RAD) / 2)) * focal_length;

    viewport_width = viewport_height * aspect_ratio;

    // z basis vector for cam
    auto basis_z = (center - target).unit_vector();

    // z cross y = x
    auto basis_x = orientation.cross(basis_z).unit_vector();

    // y = z cross x. this is already a unit vector, but
    // might as well do this for consistency
    auto basis_y = basis_z.cross(basis_x).unit_vector();

    // the horizontal vector spanning the viewport
    viewport_x = viewport_width * basis_x;

    // the vertical vector spanning the viewport
    viewport_y = viewport_height * -basis_y;

    // the horizontal vector spanning a single pixel
    pixel_delta_x = viewport_x / width;

    // the vertical vector spanning a single pixel
    pixel_delta_y = viewport_y / height;

    // top left of viewport is 1 behind the camera and half the viewport
    // width to the left and half the viewport height up from the camera
    // center
    viewport_top_left = center - (focal_length * basis_z) - (viewport_x / 2) - (viewport_y / 2);

    // transform the above into a pixel and then move to the center of that
    // pixel. still in world space
    pixel00_center = viewport_top_left + (pixel_delta_x + pixel_delta_y) * 0.5;
}

void camera::render(const hittable& world) {
    refresh();

    std::remove(output.c_str());
    std::ofstream out(output);

    out << "P3\n" << width << " " << height << "\n255\n";

    for (int row = 0; row < height; row++) {
        std::clog << "\rOn line " << row << "/" << height << std::endl;

#pragma omp parallel for ordered
        for (int col = 0; col < width; col++) {
            // find the pixel center in terms of world space
            color total_colors(0, 0, 0);

            for (int i = 0; i < antialiasing_sample_count; i++) {
                auto rand_ray = approximate_ray(row, col);
                total_colors += ray_color(rand_ray, world, 0);
            }

            total_colors *= (1.0 / antialiasing_sample_count);

#pragma omp ordered
            write_color(out, total_colors.gamma_transform());
        }
    }
}

void camera::write_color(std::ostream& out, color pixel_color) {
    static const interval zero_one(0.0, 0.9999999);

    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    out << static_cast<int>(255.999 * zero_one.clamp(r)) << ' '
        << static_cast<int>(255.999 * zero_one.clamp(g)) << ' '
        << static_cast<int>(255.999 * zero_one.clamp(b)) << '\n';
}

ray camera::approximate_ray(int i, int j) {
    vec3 offset = sample_square();
    vec3 pixel_sample =
        pixel00_center + ((j + offset.x()) * pixel_delta_x) + ((i + offset.y()) * pixel_delta_y);

    vec3 origin = center;
    vec3 direction = pixel_sample - origin;

    return ray(origin, direction);
}