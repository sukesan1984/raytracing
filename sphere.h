#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
	public:
		sphere() {}
		sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
		virtual bool hit(const ray& r, double tmin, double tmax,
				hit_record& rec) const;
		virtual bool bounding_box(double t0, double t1, aabb& output_box) const;
		vec3 center;
		double radius;
		shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max,
		hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(),  r.direction());
    double b = dot(r.direction(), oc);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
       double temp = (-b - sqrt(discriminant)) / a;
	   if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
	   }
	   temp = (-b + sqrt(discriminant)) / b;
	   if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
	   }
	}
	return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
	output_box = aabb(center - vec3(radius, radius, radius),
			center + vec3(radius, radius, radius));
	return true;
}

void get_sphere_uv(const vec3& p, double& u, double& v) {
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}

#endif
