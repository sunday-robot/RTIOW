#pragma once

#include "../material.h"

class diffuse_light : public material {
public:
	diffuse_light(std::shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(std::make_shared<solid_color>(c)) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		return false;
	}

	virtual color emitted(double u, double v, const vec3& p) const override {
		return emit->value(u, v, p);
	}

public:
	std::shared_ptr<texture> emit;
};
