#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hitable_list: public hitable {
public:
		hitable_list() {}
		hitable_list(shared_ptr<hitable> object)  { add(object); }
		void clear() { objects.clear(); }
		void add(shared_ptr<hitable> object) { objects.push_back(object); }
		virtual bool hit(const ray& r, double tmin, double tmax,
				hit_record& rec) const;
public:
	std::vector<shared_ptr<hitable>> objects;
};

bool hitable_list::hit(const ray& r, double t_min, double t_max,
		hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

#endif
