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

#include "../hittable.h"
#include <vector>

class Bvh : public hittable {
protected:
	const aabb aabb;
public:
	Bvh(::aabb aabb) : aabb(aabb) {}
	virtual ::aabb bounding_box(double exposureTime) const override {
		return aabb;
	}
};

class BvhNode : public Bvh {
	const std::shared_ptr<Bvh> left;
	const std::shared_ptr<Bvh> right;
public:
	BvhNode(::aabb aabb, std::shared_ptr<Bvh> left, std::shared_ptr<Bvh> right)
		: Bvh(aabb),
		left(left),
		right(right)
	{}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual void print(std::string indent = "") const override;
};

class BvhLeaf : public Bvh {
	const std::shared_ptr<hittable> data;
public:
	BvhLeaf(::aabb aabb, std::shared_ptr<hittable> data)
		: Bvh(aabb),
		data(data)
	{}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record* rec) const override;

	virtual void print(std::string indent = "") const override;
};

std::shared_ptr<Bvh> createBvhTree(std::vector<std::shared_ptr<hittable>> src_objects, double exposureTime);
#endif
