#include "camera.hpp"

#include "material.hpp"

color camera::ray_color(const ray& r, const hittable& tgt, int max) {
    hittable::hit_record rec;
    if (tgt.hit(r, interval::universe_positive(), rec) && max < 10) {
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
    viewport_height = viewport_width / aspect_ratio;

    // the horizontal vector spanning the viewport
    viewport_x = point3(viewport_width, 0, 0);

    // the vertical vector spanning the viewport
    viewport_y = point3(0, -viewport_height, 0);

    // the horizontal vector spanning a single pixel
    pixel_delta_x = viewport_x / width;

    // the vertical vector spanning a single pixel
    pixel_delta_y = viewport_y / height;

    // top left of viewport is 1 behind the camera and half the viewport
    // width to the left and half the viewport height up from the camera
    // center
    viewport_top_left = camera_center - vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;

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

        for (int col = 0; col < width; col++) {
            // find the pixel center in terms of world space
            color total_colors(0, 0, 0);

#pragma omp for
            for (int i = 0; i < antialiasing_sample_count; i++) {
                auto rand_ray = approximate_ray(row, col);
#pragma omp critical
                total_colors += ray_color(rand_ray, world, 0);
            }

            total_colors *= (1.0 / antialiasing_sample_count);

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

    vec3 origin = camera_center;
    vec3 direction = pixel_sample - origin;

    return ray(origin, direction);
}