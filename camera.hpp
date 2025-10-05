#include <fstream>
#include <string>

#include "hittable.hpp"
#include "utils.hpp"

class camera {
   public:
    int width = 400;
    double aspect_ratio = 16.0 / 9.0;
    double viewport_width = 2.0;
    double focal_length = 1.0;  // distance from camera sensor to viewport
    int antialiasing_sample_count = 10;
    std::string output = "output.ppm";

    int height = static_cast<int>(width / aspect_ratio);
    double viewport_height = viewport_width / aspect_ratio;

    point3 camera_center = point3(0, 0, 0);

    void render(const hittable& world) {
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

    void refresh() {
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
        viewport_top_left = camera_center - vec3(0, 0, focal_length) -
                            viewport_x / 2 - viewport_y / 2;

        // transform the above into a pixel and then move to the center of that
        // pixel. still in world space
        pixel00_center =
            viewport_top_left + (pixel_delta_x + pixel_delta_y) * 0.5;
    }

   private:
    // the horizontal vector spanning the viewport
    point3 viewport_x = point3(viewport_width, 0, 0);

    // the vertical vector spanning the viewport
    point3 viewport_y = point3(0, -viewport_height, 0);

    // the horizontal vector spanning a single pixel
    point3 pixel_delta_x = viewport_x / width;

    // the vertical vector spanning a single pixel
    point3 pixel_delta_y = viewport_y / height;

    // top left of viewport is 1 behind the camera and half the viewport
    // width to the left and half the viewport height up from the camera
    // center
    point3 viewport_top_left = camera_center - vec3(0, 0, focal_length) -
                               viewport_x / 2 - viewport_y / 2;

    // transform the above into a pixel and then move to the center of that
    // pixel. still in world space
    point3 pixel00_center =
        viewport_top_left + (pixel_delta_x + pixel_delta_y) * 0.5;

    color ray_color(const ray& r, const hittable& tgt, int max) {
        hittable::hit_record rec;
        if (tgt.hit(r, interval::universe_positive(), rec) && max < 10) {
            // where did it hit relative to the shape's normal (and if diffused,
            // that 2)
            auto rand = ray::rand_unit_vec() + rec.normal;
            return 0.3 * ray_color(ray(rec.p, rand), tgt, max + 1);
            // return 0.5 * (rec.normal + color(1, 1, 1));
        }

        // sky
        double a = 0.5 * (r.direction().unit_vector().y() + 1);
        return (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
    }

    void write_color(std::ostream& out, color pixel_color) {
        static const interval zero_one(0.0, 0.9999999);

        double r = pixel_color.x();
        double g = pixel_color.y();
        double b = pixel_color.z();

        out << static_cast<int>(255.999 * zero_one.clamp(r)) << ' '
            << static_cast<int>(255.999 * zero_one.clamp(g)) << ' '
            << static_cast<int>(255.999 * zero_one.clamp(b)) << '\n';
    }

    ray approximate_ray(int i, int j) {
        vec3 offset = sample_square();
        vec3 pixel_sample = pixel00_center +
                            ((j + offset.x()) * pixel_delta_x) +
                            ((i + offset.y()) * pixel_delta_y);

        vec3 origin = camera_center;
        vec3 direction = pixel_sample - origin;

        return ray(origin, direction);
    }

    vec3 sample_square() const {
        return vec3(rand_double() - 0.5, rand_double() - 0.5,
                    0);  // z coord doesn't matter
    }
};