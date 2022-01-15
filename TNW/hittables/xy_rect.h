#pragma once
#include "../hittable.h"

class xy_rect : public hittable {
public:
	xy_rect() {}

	xy_rect(
		double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> mat
	) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		return  aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
	}

	virtual void print(std::string indent = "") const override;

public:
	std::shared_ptr<material> mp;
	double x0, x1, y0, y1, k;
};
