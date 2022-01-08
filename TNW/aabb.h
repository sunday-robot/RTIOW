#ifndef AABB_H
#define AABB_H
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

#include "ray.h"

class aabb {
public:
	aabb() {}
	aabb(const vec3& a, const vec3& b) { minimum = a; maximum = b; }

	vec3 min() const { return minimum; }
	vec3 max() const { return maximum; }

	bool hit(const ray& r, double t_min, double t_max) const {
		{
			auto a = (minimum.x - r.origin.x) / r.direction.x;
			auto b = (maximum.x - r.origin.x) / r.direction.x;
			t_min = fmax(fmin(a, b), t_min);
			t_max = fmin(fmax(a, b), t_max);
			if (t_max <= t_min)
				return false;
		}
		{
			auto a = (minimum.y - r.origin.y) / r.direction.y;
			auto b = (maximum.y - r.origin.y) / r.direction.y;
			t_min = fmax(fmin(a, b), t_min);
			t_max = fmin(fmax(a, b), t_max);
			if (t_max <= t_min)
				return false;
		}
		{
			auto a = (minimum.z - r.origin.z) / r.direction.z;
			auto b = (maximum.z - r.origin.z) / r.direction.z;
			t_min = fmax(fmin(a, b), t_min);
			t_max = fmin(fmax(a, b), t_max);
			if (t_max <= t_min)
				return false;
		}
		return true;
	}

	double area() const {
		auto a = maximum.x - minimum.x;
		auto b = maximum.y - minimum.y;
		auto c = maximum.z - minimum.z;
		return 2 * (a * b + b * c + c * a);
	}

	int longest_axis() const {
		auto a = maximum.x - minimum.x;
		auto b = maximum.y - minimum.y;
		auto c = maximum.z - minimum.z;
		if (a > b && a > c)
			return 0;
		else if (b > c)
			return 1;
		else
			return 2;
	}

public:
	vec3 minimum;
	vec3 maximum;
};
#endif
