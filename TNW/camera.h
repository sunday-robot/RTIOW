#ifndef CAMERA_H
#define CAMERA_H
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

//#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"
#include "random_utils.h"
#include "degrees_to_radians.h"

class camera {
public:
	camera() : camera(vec3(0, 0, -1), vec3(0, 0, 0), vec3(0, 1, 0), 40, 1, 0, 10) {}

	camera(
		vec3 lookfrom,
		vec3 lookat,
		vec3   vup,
		double vfov, // vertical field-of-view in degrees
		double aspect_ratio,
		double aperture,
		double _exposureTime = 0
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		auto w = unit_vector(lookfrom - lookat);
		_u = unit_vector(cross(vup, w));
		_v = cross(w, _u);

		auto dist_to_focus = (lookat - lookfrom).length();
		_origin = lookfrom;
		_horizontal = dist_to_focus * viewport_width * _u;
		_vertical = dist_to_focus * viewport_height * _v;
		_lower_left_corner = _origin - _horizontal / 2 - _vertical / 2 - dist_to_focus * w;

		_lens_radius = aperture / 2;
		_exposureTime = _exposureTime;
	}

	ray get_ray(double s, double t) const {
		vec3 rd = _lens_radius * random_in_unit_disk();
		vec3 offset = _u * rd.x + _v * rd.y;
		return ray(
			_origin + offset,
			_lower_left_corner + s * _horizontal + t * _vertical - _origin - offset,
			random_double(-_exposureTime / 2, _exposureTime / 2)
		);
	}

private:
	vec3 _origin;
	vec3 _lower_left_corner;
	vec3 _horizontal;
	vec3 _vertical;
	vec3 _u;
	vec3 _v;
	double _lens_radius;
	double _exposureTime;
};
#endif
