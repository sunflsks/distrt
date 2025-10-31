#include <fstream>
#include <string>

#include "hittable.hpp"
#include "utils.hpp"

class Camera {
   public:
    int width = 200;
    double aspect_ratio = 16.0 / 9.0;
    double viewport_width = 2.0;
    int antialiasing_sample_count = 15;
    int vfov = 90;
    std::string output = "output.ppm";

    int height = static_cast<int>(width / aspect_ratio);
    double viewport_height = viewport_width / aspect_ratio;

    Point3 center = Point3(0, 0, 0);
    Point3 target = Point3(0, 0, -1);
    Point3 orientation = Point3(0, 1, 0);  // vector normal to the top surface of our camera

    void render(const Hittable& world);

    void refresh();

   private:
    // distance from camera sensor to viewport, will be recalculated
    double focal_length = 1.0;

    // the horizontal vector spanning the viewport
    Point3 viewport_x = Point3(viewport_width, 0, 0);

    // the vertical vector spanning the viewport
    Point3 viewport_y = Point3(0, -viewport_height, 0);

    // the horizontal vector spanning a single pixel
    Point3 pixel_delta_x = viewport_x / width;

    // the vertical vector spanning a single pixel
    Point3 pixel_delta_y = viewport_y / height;

    // top left of viewport is 1 behind the camera and half the viewport
    // width to the left and half the viewport height up from the camera
    // center
    Point3 viewport_top_left = center - Vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;

    // transform the above into a pixel and then move to the center of that
    // pixel. still in world space
    Point3 pixel00_center = viewport_top_left + (pixel_delta_x + pixel_delta_y) * 0.5;

    Color ray_color(const Ray& r, const Hittable& tgt, int max);

    void write_color(std::ostream& out, Color pixel_color);

    Ray approximate_ray(int i, int j);

    Vec3 sample_square() const {
        return Vec3(rand_double() - 0.5, rand_double() - 0.5,
                    0);  // z coord doesn't matter
    }
};