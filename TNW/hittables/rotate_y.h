#pragma once

#include "../hittable.h"
#include "../hit_record.h"

class rotate_y : public hittable {
public:
	rotate_y(std::shared_ptr<hittable> p, double angle);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual bool bounding_box(double exposureTime, aabb* output_box) const override {
		*output_box = bbox;
		return hasbox;
	}

public:
	std::shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
};
