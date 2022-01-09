#include "Renderer.h"
#include <chrono>
#include <iostream>
#include <omp.h>
#include "ray.h"
#include "random_utils.h"
#include "hittable.h"
#include "constants.h"
#include "hit_record.h"
#include "material.h"

color Renderer::ray_color(const ray& r, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// If the ray hits nothing, return the background color.
	if (!world->hit(r, 0.001, infinity, &rec))
		return backgroundColor;

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, depth - 1);
}

static unsigned char convert(double value) {
	return (unsigned char)(255 * std::min(value, 1.0));
}

unsigned char* Renderer::render(const camera& camera, int image_width, int image_height)
{
	auto image = new unsigned char[image_width * 3 * image_height];
	auto start = std::chrono::system_clock::now();
	for (int j = 0; j < image_height; j++) {
		std::cerr << "\rScanlines remaining: " << j << '.' << image_height << std::flush;
		for (int i = 0; i < image_width; ++i) {
			double red = 0;
			double green = 0;
			double blue = 0;
#pragma omp parallel for
			for (int s = 0; s < samplesPerPixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = camera.get_ray(u, v);
				auto c = ray_color(r, maxDepth);
				red += c.r;
				green += c.g;
				blue += c.b;
			}
			color pixel_color(red / samplesPerPixel, green / samplesPerPixel, blue / samplesPerPixel);
			auto index = 3 * image_width * (image_height - 1 - j) + 3 * i;
			image[index] = convert(pixel_color.b);
			image[index + 1] = convert(pixel_color.g);
			image[index + 2] = convert(pixel_color.r);
		}
	}
	auto end = std::chrono::system_clock::now();
	std::cerr << "time=" << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
	return image;
}
