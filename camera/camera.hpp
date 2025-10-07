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

    void render(const hittable& world);

    void refresh();

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
    point3 viewport_top_left =
        camera_center - vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;

    // transform the above into a pixel and then move to the center of that
    // pixel. still in world space
    point3 pixel00_center = viewport_top_left + (pixel_delta_x + pixel_delta_y) * 0.5;

    color ray_color(const ray& r, const hittable& tgt, int max);

    void write_color(std::ostream& out, color pixel_color);

    ray approximate_ray(int i, int j);

    vec3 sample_square() const {
        return vec3(rand_double() - 0.5, rand_double() - 0.5,
                    0);  // z coord doesn't matter
    }
};