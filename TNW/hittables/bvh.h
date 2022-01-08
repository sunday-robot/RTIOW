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
		: bvh_node(list.objects, 0, list.objects.size(), exposureTime)
	{}

	bvh_node(
		const std::vector<std::shared_ptr<hittable>>& src_objects,
		size_t start, size_t end, double exposureTime);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double exposureTime, aabb& output_box) const override;

public:
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb box;
};
#endif
