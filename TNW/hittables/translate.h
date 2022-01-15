#pragma once

#include "../hittable.h"

class translate : public hittable {
public:
	translate(std::shared_ptr<hittable> p, const vec3& displacement)
		: ptr(p), offset(displacement) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override;

public:
	std::shared_ptr<hittable> ptr;
	vec3 offset;
};
