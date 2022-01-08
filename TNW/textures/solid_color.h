#pragma once

#include "../texture.h"
#include "../vec3.h"

class solid_color : public texture {
public:
	solid_color() : color_value(0,0,0) {}
	solid_color(color c) : color_value(c) {}

	solid_color(double red, double green, double blue)
		: solid_color(color(red, green, blue)) {}

	virtual color value(double u, double v, const vec3& p) const override {
		return color_value;
	}

private:
	color color_value;
};
