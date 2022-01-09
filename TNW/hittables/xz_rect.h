#pragma once
#include "../hittable.h"

class xz_rect : public hittable {
public:
	xz_rect() {}

	xz_rect(
		double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<material> mat
	) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual bool bounding_box(double exposureTime, aabb* output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Y
		// dimension a small amount.
		*output_box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
		return true;
	}

public:
	std::shared_ptr<material> mp;
	double x0, x1, z0, z1, k;
};
