#pragma once

#include <vector>
#include "hittable.h"
#include "color.h"
#include "camera.h"

class Scene
{
public:
	std::vector<std::shared_ptr<hittable>> _world;
	color _backgroundColor;
	camera _camera;
	int _image_width;
	int _image_height;
	int _samples_per_pixel;

public:
	Scene(std::vector<std::shared_ptr<hittable>> world, color backgroundColor, camera camera, int image_width, int image_height, int samples_per_pixel)
		: _world(world), _backgroundColor(backgroundColor), _camera(camera), _image_width(image_width), _image_height(image_height), _samples_per_pixel(samples_per_pixel)
	{}
};

