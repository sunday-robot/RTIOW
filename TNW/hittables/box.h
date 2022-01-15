#ifndef BOX_H
#define BOX_H
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

#include "hittable_list.h"

class box : public hittable {
public:
	box() {}
	box(const vec3& p0, const vec3& p1, std::shared_ptr<material> ptr);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override {
		return aabb(box_min, box_max);
	}

public:
	vec3 box_min;
	vec3 box_max;
	hittable_list sides;
};
#endif
