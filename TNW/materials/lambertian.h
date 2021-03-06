#pragma once

#include "../material.h"

class lambertian : public material {
public:
	lambertian(const color& a) : albedo(std::make_shared<solid_color>(a)) {}
	lambertian(std::shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction, r_in.time);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	std::shared_ptr<texture> albedo;
};
