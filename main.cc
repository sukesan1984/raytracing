#include <iostream>
#include "camera.h"
#include "sphere.h"
#include <stdlib.h>
#include "material.h"
#include "hitable_list.h"
#include "color.h"

vec3 ray_color(const ray& r, hitable_list world, int depth) {
    hit_record rec;
    if (world.hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable_list random_scene() {
    int n = 500;
    int num_a = 3;
    int num_b = 3;
    hitable_list world;
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, new lambertian(
                vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for (int a = -num_a; a < num_a; a++) {
        for (int b = -num_b; b < num_b; b++) {
            double choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    world.add(make_shared<sphere>(center, 0.2, new lambertian(
                                vec3(drand48() * drand48(), drand48() * drand48(),
                                    drand48() * drand48()))));
                }
                else if (choose_mat < 0.95) {
                    world.add(make_shared<sphere>(
                            center,
                            0.2,
                            new metal(
                                vec3(
                                    0.5 * (1 + drand48()),
                                    0.5 * (1 + drand48()),
                                    0.5 * (1 + drand48())
                                ),
                            0.5 * drand48())));
                }
                else {
                    world.add(make_shared<sphere>(center, 0.2, new dielectric(1.5)));
                }
            }
        }
    }
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0,
            new lambertian(vec3(0.4, 0.2, 0.1))));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0,
            new metal(vec3(0.7, 0.6, 0.5), 0.0)));
    return world;
}

int main() {
    int nx = 400;
    int ny = 200;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[4];
    vec3 lookfrom(12, 2, 3);
    vec3 lookat(4, 1, 1);

    double dist_to_focus = (lookfrom - lookat).length();
    double aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus);
    //double R = cos(M_PI / 4);
    //list[0] = new sphere(vec3(-R, 0, -1), R,
    //        new lambertian(vec3(0, 0, 1)));
    //list[1] = new sphere(vec3(R, 0, -1), R,
    //        new lambertian(vec3(1, 0, 0)));
    //list[0] = new sphere(vec3(0, 0, -1), 0.5,
    //        new lambertian(vec3(0.1, 0.2, 0.5)));
    //list[1] = new sphere(vec3(0, -100.5, -1), 100,
    //        new lambertian(vec3(0.8, 0.8, 0.0)));
    //list[2] = new sphere(vec3(1, 0, -1), 0.5,
    //        new metal(vec3(0.8, 0.6, 0.2), 0.3));
    //list[3] = new sphere(vec3(-1, 0, -1), -0.45,
    //        new dielectric(1.5));
    //hitable *world = new hitable_list(list, 4);
    //hitable *world = new hitable_list(list, 2);
    auto world = random_scene();
    for (int j = ny - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                double u = double(i + drand48()) /double(nx);
                double v = double(j + drand48()) / double(ny);
                ray r = cam.get_ray(u, v);
                col += ray_color(r, world, 0);
            }
            col /= double(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            write_color(std::cout, col);
        }
    }
}
