#pragma once
#include "../hittable.h"

class yz_rect : public hittable {
public:
	yz_rect() {}

	yz_rect(
		double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<material> mat
	) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual bool bounding_box(double exposureTime, aabb* output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the X
		// dimension a small amount.
		*output_box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
		return true;
	}

public:
	std::shared_ptr<material> mp;
	double y0, y1, z0, z1, k;
};
