#pragma once

#include "../hittable.h"
#include "../hit_record.h"

class rotate_y : public hittable {
public:
	rotate_y(std::shared_ptr<hittable> p, double angle);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override {
		return bbox;
	}

	virtual void print(std::string indent = "") const override;

public:
	std::shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	aabb bbox;
};
