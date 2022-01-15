#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H
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

#include "../hittable.h"

class moving_sphere : public hittable {
public:
	moving_sphere() {}
	moving_sphere(
		vec3 c, vec3 v, double r, std::shared_ptr<material> m)
		: center(c), velocity(v), radius(r), mat_ptr(m)
	{};

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override;

	vec3 centerAt(double time) const;

public:
	vec3 center;
	vec3 velocity;
	double radius;
	std::shared_ptr<material> mat_ptr;
};
#endif
