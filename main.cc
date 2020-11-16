#include <iostream>
#include "rtweekend.h"
#include "rtw_stb_image.h"
#include "camera.h"
#include "sphere.h"
#include "aarect.h"
#include <stdlib.h>
#include "material.h"
#include "hitable_list.h"
#include "color.h"
#include "bvh.h"

vec3 ray_color(const ray& r, const color& background, hitable_list world, int depth) {
    hit_record rec;
    if (depth <= 0)
        return color(0, 0, 0);
    if (!world.hit(r, 0.001, MAXFLOAT, rec))
        return background;
    ray scattered;
    vec3 attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

hitable_list simple_light() {
    hitable_list objects;
    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(
        point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext))
    );
    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 2, make_shared<lambertian>(pertext))
    );

    auto difflight = make_shared<diffuse_light>(make_shared<solid_color>(4, 4, 4));
    //objects.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    return objects;
}

hitable_list earth() {
    auto earth_texture = make_shared<image_texture>("earth.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);
    return hitable_list(globe);
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

hitable_list two_perlin_spheres() {
    hitable_list objects;

    auto pertext = make_shared<noise_texture>(5);
    //auto pertext = make_shared<turbulence_texture>();
    objects.add(make_shared<sphere>(
        point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext))
    );
    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 2, make_shared<lambertian>(pertext))
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
    //int nx = 400;
    //int ny = 200;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 384;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 100;
    const int max_depth = 50;
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    point3 lookfrom(26, 3, 6);
    point3 lookat(0, 2, 0);
    vec3 vup(0, 1, 0);
    double dist_to_focus = 10.0;
    double aperture = 0.0;
    const color background(0, 0, 0);
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    auto world = simple_light();
    ///auto world = random_scene();
    //auto world = two_spheres();
    //auto world = two_perlin_spheres();
    //auto world = earth();
    //auto world = cornell_box();
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) /double(image_width - 1);
                double v = (j + random_double()) / double(image_height - 1);
                ray r = cam.get_ray(u, v);
                col += ray_color(r, background, world, max_depth);
            }
            //col /= double(ns);
            //col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            write_color(std::cout, col, samples_per_pixel);
        }
    }
}
