//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include <iostream>
#include <chrono>
#include "camera.h"
#include "write_color.h"
#include "libBmp.h"
#include "constants.h"
#include "hittables/box.h"
#include "hittables/bvh.h"
#include "hittables/constant_medium.h"
#include "hittables/hittable_list.h"
#include "hittables/moving_sphere.h"
#include "hittables/rotate_y.h"
#include "hittables/sphere.h"
#include "hittables/translate.h"
#include "hittables/xy_rect.h"
#include "hittables/xz_rect.h"
#include "hittables/yz_rect.h"
#include "materials/dielectric.h"
#include "materials/diffuse_light.h"
#include "materials/isotropic.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "textures/checker_texture.h"
#include "textures/image_texture.h"
#include "textures/noise_texture.h"
#include "Renderer.h"

using namespace std;

std::vector<std::shared_ptr<hittable>> random_scene() {
	std::vector<std::shared_ptr<hittable>> world;

	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

	world.push_back(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = randomColor() * randomColor();
					sphere_material = make_shared<lambertian>(albedo);
					auto velocity = vec3(0, random_double(0, .5), 0);
					world.push_back(make_shared<moving_sphere>(
						center, velocity, 0.2, sphere_material));
				} else if (choose_mat < 0.95) {
					// metal
					auto albedo = randomColor(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.push_back(make_shared<sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.push_back(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.push_back(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.push_back(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.push_back(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

	return world;
}

std::vector<std::shared_ptr<hittable>> two_spheres() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

	objects.push_back(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.push_back(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	return objects;
}

std::vector<std::shared_ptr<hittable>> two_perlin_spheres() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.push_back(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.push_back(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	return objects;
}

std::vector<std::shared_ptr<hittable>> earth() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto earth_texture = make_shared<image_texture>("earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);
	objects.push_back(globe);

	return objects;
}

std::vector<std::shared_ptr<hittable>> simple_light() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.push_back(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.push_back(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	objects.push_back(make_shared<sphere>(vec3(0, 7, 0), 2, difflight));
	objects.push_back(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

std::vector<std::shared_ptr<hittable>> cornell_box() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	objects.push_back(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.push_back(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.push_back(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.push_back(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.push_back(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.push_back(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.push_back(box1);

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.push_back(box2);

	return objects;
}

std::vector<std::shared_ptr<hittable>> cornell_smoke() {
	std::vector<std::shared_ptr<hittable>> objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(7, 7, 7));

	objects.push_back(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.push_back(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.push_back(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
	objects.push_back(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.push_back(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.push_back(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));

	shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	objects.push_back(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
	objects.push_back(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

	return objects;
}

std::vector<std::shared_ptr<hittable>> final_scene() {
	std::vector<std::shared_ptr<hittable>> objects;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			objects.push_back(make_shared<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}

	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	objects.push_back(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center = vec3(400, 400, 200);
	auto velocity = vec3(30, 0, 0);
	auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	objects.push_back(make_shared<moving_sphere>(center, velocity, 50, moving_sphere_material));

	objects.push_back(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.push_back(make_shared<sphere>(
		vec3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = make_shared<sphere>(vec3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.push_back(boundary);
	objects.push_back(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(vec3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	objects.push_back(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
	objects.push_back(make_shared<sphere>(vec3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.push_back(make_shared<sphere>(vec3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	{
		std::vector<std::shared_ptr<hittable>> boxes2;
		auto white = make_shared<lambertian>(color(.73, .73, .73));
		int ns = 1000;
		for (int j = 0; j < ns; j++) {
			boxes2.push_back(make_shared<sphere>(randomVec3(0, 165), 10, white));
		}
		objects.push_back(make_shared<translate>(
			make_shared<rotate_y>(
				make_shared<bvh_node>(boxes2, 1.0), 15),
			vec3(-100, 270, 395)
			)
		);
	}

	return objects;
}

static unsigned char convert(double value) {
	return (unsigned char)(255 * std::min(value, 1.0));
}

int main() {

	// Image

	int image_width = 1280;
	int image_height = 720;
	int samples_per_pixel = 10;
	int max_depth = 50;

	// World

	std::vector<std::shared_ptr<hittable>> world;

	vec3 lookfrom;
	vec3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;
	color background(0, 0, 0);

	switch (8) {
	case 1:
		world = random_scene();
		background = color(0.70, 0.80, 1.00);
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;

	case 2:
		world = two_spheres();
		background = color(0.70, 0.80, 1.00);
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;

	case 3:
		world = two_perlin_spheres();
		background = color(0.70, 0.80, 1.00);
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;

	case 4:
		world = earth();
		background = color(0.70, 0.80, 1.00);
		lookfrom = vec3(0, 0, 12);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;

	case 5:
		world = simple_light();
		samples_per_pixel = 400;
		lookfrom = vec3(26, 3, 6);
		lookat = vec3(0, 2, 0);
		vfov = 20.0;
		break;

	default:
	case 6:
		world = cornell_box();
		image_width = 600;
		samples_per_pixel = 200;
		lookfrom = vec3(278, 278, -800);
		lookat = vec3(278, 278, 0);
		vfov = 40.0;
		break;

	case 7:
		world = cornell_smoke();
		image_width = 600;
		samples_per_pixel = 200;
		lookfrom = vec3(278, 278, -800);
		lookat = vec3(278, 278, 0);
		vfov = 40.0;
		break;

	case 8:
		world = final_scene();
#if true
		image_width = 1280;
		image_height = 720;
#else
		image_width = 320;
		image_height = 180;
#endif
#if false
		samples_per_pixel = 100;
#else
		samples_per_pixel = 10;
#endif
		lookfrom = vec3(478, 278, -600);
		lookat = vec3(278, 278, 0);
		vfov = 40.0;
		break;
	}

	auto bvh_world = new bvh_node(world, 1.0);

	// Camera

	const vec3 vup(0, 1, 0);
	auto dist_to_focus = (lookat - lookfrom).length();

	auto aspect_ratio = ((double)image_width) / image_height;
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 1.0);

	// Render
	Renderer renderer(50, samples_per_pixel);
	renderer.setWorld(bvh_world);
	renderer.setBackground(background);
	auto image = renderer.render(cam, image_width, image_height);
	bmpSave("scene.bmp", image, image_width, image_height);
	delete[] image;
	std::cerr << "hit enter to exit." << std::endl;
	std::cin.get();
}
