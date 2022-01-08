#pragma once

#include "../texture.h"
#include "../vec3.h"
#include "../perlin.h"

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(double sc) : scale(sc) {}

	virtual color value(double u, double v, const vec3& p) const override {
		// return color(1,1,1)*0.5*(1 + noise.turb(scale * p));
		// return color(1,1,1)*noise.turb(scale * p);
		return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}

public:
	perlin noise;
	double scale;
};
