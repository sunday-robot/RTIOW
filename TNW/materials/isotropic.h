#pragma once

#include "../material.h"
#include "../textures/solid_color.h"
#include "../random_utils.h"

class isotropic : public material {
public:
	isotropic(color c) : albedo(std::make_shared<solid_color>(c)) {}
	isotropic(std::shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		scattered = ray(rec.p, random_in_unit_sphere(), r_in.time);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	std::shared_ptr<texture> albedo;
};
