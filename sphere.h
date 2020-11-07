#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
	public:
		sphere() {}
		sphere(point3 cen, double r, material *m) : center(cen), radius(r), mat_ptr(m) {};
		virtual bool hit(const ray& r, double tmin, double tmax,
				hit_record& rec) const;
		vec3 center;
		double radius;
		material *mat_ptr;
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

#endif
