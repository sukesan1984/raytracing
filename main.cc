#include <iostream>
#include "rtweekend.h"
#include "camera.h"
#include "sphere.h"
#include <stdlib.h>
#include "material.h"
#include "hitable_list.h"
#include "color.h"
#include "bvh.h"

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

hitable_list two_spheres() {
    hitable_list objects;

    auto checker = make_shared<checker_texture>(
        make_shared<solid_color>(0.2, 0.3, 0.1),
        make_shared<solid_color>(0.9, 0.9, 0.9)
    );

    objects.add(
        make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker))
    );
    objects.add(
        make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker))
    );
    return objects;
}

hitable_list random_scene() {
    int n = 500;
    int num_a = 5;
    int num_b = 5;
    hitable_list world;
    auto checker = make_shared<checker_texture>(
        make_shared<solid_color>(0.2, 0.3, 0.1),
        make_shared<solid_color>(0.9, 0.9, 0.9)
    );
    //world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(make_shared<solid_color>(0.5, 0.5, 0.5))));
    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker))
    );
    int i = 1;
    for (int a = -num_a; a < num_a; a++) {
        for (int b = -num_b; b < num_b; b++) {
            double choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(
                                make_shared<solid_color>(random_double() * random_double(), random_double() * random_double(),
                                    random_double() * random_double()))));
                }
                else if (choose_mat < 0.95) {
                    world.add(make_shared<sphere>(
                            center,
                            0.2,
                            make_shared<metal>(
                                vec3(
                                    0.5 * (1 + random_double()),
                                    0.5 * (1 + random_double()),
                                    0.5 * (1 + random_double())
                                ),
                            0.5 * random_double())));
                }
                else {
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0,
            make_shared<lambertian>(make_shared<solid_color>(0.4, 0.2, 0.1))));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0,
            make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));
    return hitable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

int main() {
    int nx = 400;
    int ny = 200;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[4];
    //point3 lookfrom(12, 2, 3);
    //point3 lookat(4, 1, 1);
    //double dist_to_focus = (lookfrom - lookat).length();
    //double aperture = 0.1;
    //camera cam(lookfrom, lookat, vup, 20, double(nx) / double(ny), aperture, dist_to_focus);
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    double dist_to_focus = 10.0;
    double aperture = 0.0;
    camera cam(lookfrom, lookat, vup, 20, double(nx) / double(ny), aperture, dist_to_focus);
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
    ///auto world = random_scene();
    auto world = two_spheres();
    for (int j = ny - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                double u = (i + random_double()) /double(nx);
                double v = (j + random_double()) / double(ny);
                ray r = cam.get_ray(u, v);
                col += ray_color(r, world, 0);
            }
            //col /= double(ns);
            //col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            write_color(std::cout, col, ns);
        }
    }
}
