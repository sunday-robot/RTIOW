#include "bvh.h"
#include <algorithm>
#include "../surrounding_box.h"
#include "../random_utils.h"

bool BvhNode::hit(const ray& r, double t_min, double t_max, hit_record* rec) const
{
	if (!aabb.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max, rec);
	if (hit_left) {
		right->hit(r, t_min, rec->t, rec);
		return true;
	} else {
		return right->hit(r, t_min, t_max, rec);
	}
}

void BvhNode::print(std::string indent) const
{
	std::cout << indent << "aabb = " << aabb.to_string() << std::endl;
	std::cout << indent << "left:" << std::endl;
	left->print(indent + "  ");
	std::cout << indent << "right:" << std::endl;
	right->print(indent + "  ");
}

bool BvhLeaf::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	return aabb.hit(r, t_min, t_max)
		&& data->hit(r, t_min, t_max, rec);
}

void BvhLeaf::print(std::string indent) const
{
	std::cout << indent << "aabb = " << aabb.to_string() << std::endl;
	data->print(indent);
}

static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a = a->bounding_box(0);
	aabb box_b = b->bounding_box(0);
	return box_a.min().x < box_b.min().x;
}

static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a = a->bounding_box(0);
	aabb box_b = b->bounding_box(0);
	return box_a.min().y < box_b.min().y;
}

static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a = a->bounding_box(0);
	aabb box_b = b->bounding_box(0);
	return box_a.min().z < box_b.min().z;
}

static std::shared_ptr<Bvh> createBvh(std::vector<std::shared_ptr<hittable>>* src_objects, double exposureTime, size_t start, size_t end)
{
	if (end - start == 1) {
		auto aabb = (*src_objects)[start]->bounding_box(exposureTime);
		return std::make_shared<BvhLeaf>(aabb, (*src_objects)[start]);
	} else {
		auto axis = random_int(0, 2);
		auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
			: box_z_compare;
		std::sort(src_objects->begin() + start, src_objects->begin() + end, comparator);
		auto mid = (start + end) / 2;
		auto left = createBvh(src_objects, exposureTime, start, mid);
		auto right = createBvh(src_objects, exposureTime, mid, end);
		auto aabb = surrounding_box(left->bounding_box(exposureTime), right->bounding_box(exposureTime));
		return std::make_shared<BvhNode>(aabb, left, right);
	}
}

std::shared_ptr<Bvh> createBvhTree(std::vector<std::shared_ptr<hittable>> src_objects, double exposureTime)
{
	return createBvh(&src_objects, exposureTime, 0, src_objects.size());
}
