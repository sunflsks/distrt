#include "camera.hpp"

#include <charconv>
#include <execution>
#include <ranges>

#include "material/material.hpp"

Color Camera::ray_color(const Ray& r, const Hittable& tgt, int max) {
    Hittable::hit_record rec;
    if (tgt.hit(r, Interval::universe_positive(), rec) && max < 20) {
        auto scattered_ray = rec.mat->scatter(r, rec);
        if (scattered_ray) {
            return scattered_ray->attenuation * ray_color(scattered_ray->scattered, tgt, max + 1);
        }
    }

    // sky
    double a = 0.5 * (r.direction().unit_vector().y() + 1);
    return (1 - a) * Color(1, 1, 1) + a * Color(0.5, 0.7, 1.0);
}

void Camera::refresh() {
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

void Camera::render(const Hittable& world) {
    refresh();
    std::clog << "Rendering frame..." << std::endl;

    [[maybe_unused]]
    static auto clear_old_value = [&]() { return std::remove(output.c_str()); }();

    std::ofstream out(output, std::ios::app);

    out << "P3\n" << width << " " << height << "\n255\n";

    std::vector<int> col_cnt(width);
    std::ranges::iota(col_cnt, 0);

    std::vector<Color> colors(height * width);
    for (int row = 0; row < height; row++) {
        std::for_each(std::execution::par_unseq, col_cnt.begin(), col_cnt.end(), [&](int col) {
            // find the pixel center in terms of world space
            Color total_colors(0, 0, 0);

            for (int i = 0; i < antialiasing_sample_count; i++) {
                auto rand_ray = approximate_ray(row, col);
                total_colors += ray_color(rand_ray, world, 0);
            }

            total_colors *= (1.0 / antialiasing_sample_count);

            colors[(row * width) + col] = total_colors.gamma_transform();
        });
    }

    write_colors(out, colors);
}

void Camera::write_colors(std::ostream& out, const std::vector<Color>& colors) {
    static const Interval ZERO_ONE(0.0, 0.9999999);
    constexpr int MAX_PIXEL_DIGITS = 3;

    std::vector<char> ppm_buf(colors.size() * (MAX_PIXEL_DIGITS + 1) * 3,
                              ' ');  // 3 nums, each with max 3 digits + 1 whitespace

    auto ppm_data_ptr = ppm_buf.data();
    size_t sz = 0;
    for (const auto& color : colors) {
        double r = color.x();
        double g = color.y();
        double b = color.z();

        for (auto color : {r, g, b}) {
            auto [new_ppm_data_ptr, err] =
                std::to_chars(ppm_data_ptr,
                              ppm_data_ptr + MAX_PIXEL_DIGITS,
                              static_cast<int>(255.999 * ZERO_ONE.clamp(color)));

            if (err != std::errc{}) {
                auto ec = std::make_error_code(err);
                std::string errormsg = "Unable to write to file: " + ec.message();
                throw std::runtime_error(errormsg);
            }

            sz += (new_ppm_data_ptr - ppm_data_ptr);

            ppm_data_ptr = new_ppm_data_ptr;
            *ppm_data_ptr = ' ';
            sz++;
            ppm_data_ptr++;
        }

        // points to where we would write a new number, so we need to edit the previous whitespace
        // and turn it into a newline.

        *(ppm_data_ptr - 1) = '\n';
    }

    out.write(ppm_buf.data(), sz);
}

Ray Camera::approximate_ray(int i, int j) {
    Vec3 offset = sample_square();
    Vec3 pixel_sample =
        pixel00_center + ((j + offset.x()) * pixel_delta_x) + ((i + offset.y()) * pixel_delta_y);

    Vec3 origin = center;
    Vec3 direction = pixel_sample - origin;

    return Ray(origin, direction);
}