#pragma once

#include "../texture.h"
#include "libBmp.h"
#include "../vec3.h"
#include "../clamp.h"

class image_texture : public texture {
public:
	image_texture()
		: data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

	image_texture(const char* filename) {
		if (!bmpLoad(filename, &data, &width, &height)) {
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}

		bytes_per_scanline = 3 * width;
	}

	~image_texture() {
		delete[] data;
	}

	virtual color value(double u, double v, const vec3& p) const override {
		// If we have no texture data, then return solid cyan as a debugging aid.
		if (data == nullptr)
			return color(0, 1, 1);

		// Clamp input texture coordinates to [0,1] x [1,0]
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= width)  i = width - 1;
		if (j >= height) j = height - 1;

		const auto color_scale = 1.0 / 255.0;
		auto pixel = data + j * bytes_per_scanline + i * 3;

		return color(color_scale * pixel[2], color_scale * pixel[1], color_scale * pixel[0]);
	}

private:
	unsigned char* data;
	int width, height;
	int bytes_per_scanline;
};
