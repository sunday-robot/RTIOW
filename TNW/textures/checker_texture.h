#pragma once

#include "solid_color.h"

class checker_texture : public texture {
public:
	checker_texture() {}

	checker_texture(std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd)
		: even(_even), odd(_odd) {}

	checker_texture(color c1, color c2)
		: even(std::make_shared<solid_color>(c1)), odd(std::make_shared<solid_color>(c2)) {}

	virtual color value(double u, double v, const vec3& p) const override {
		auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

public:
	std::shared_ptr<texture> odd;
	std::shared_ptr<texture> even;
};
