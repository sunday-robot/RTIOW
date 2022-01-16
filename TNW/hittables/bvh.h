#ifndef BVH_H
#define BVH_H
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

#include <algorithm>
#include "hittable_list.h"

class bvh_node : public hittable {
public:
	bvh_node() {};

	bvh_node(const hittable_list& list, double exposureTime)
		: bvh_node(list.objects, exposureTime)
	{}

	bvh_node(std::vector<std::shared_ptr<hittable>> src_objects, double exposureTime)
		: bvh_node(&src_objects, 0, src_objects.size(), exposureTime)
	{}

	bvh_node(
		std::vector<std::shared_ptr<hittable>>* src_objects,
		size_t start, size_t end, double exposureTime);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual aabb bounding_box(double exposureTime) const override;

	virtual void print(std::string indent = "") const override;

private:
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb aabb;
};
#endif
