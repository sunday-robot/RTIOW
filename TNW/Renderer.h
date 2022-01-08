#pragma once

#include "hittable.h"
#include "color.h"
#include "camera.h"

class Renderer
{
	const int maxDepth;
	const int samplesPerPixel;
	const hittable* world;
	color backgroundColor;

	color ray_color(const ray& r, int depth);

public:
	Renderer(int maxDepth, int samplesPerPixel) : maxDepth(maxDepth), samplesPerPixel(samplesPerPixel) {}
	void setWorld(const hittable* value) { world = value; }
	void setBackground(const color& value) { backgroundColor = value; }
	unsigned char* render(const camera& camera, int image_width, int image_height);
};

