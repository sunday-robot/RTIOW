#include "bvh.h"
#include "../surrounding_box.h"
#include "../random_utils.h"

bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a;
	aabb box_b;
	a->bounding_box(0, &box_a);
	b->bounding_box(0, &box_b);
	return box_a.min().x < box_b.min().x;
}

bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a;
	aabb box_b;
	a->bounding_box(0, &box_a);
	b->bounding_box(0, &box_b);
	return box_a.min().y < box_b.min().y;
}

bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	aabb box_a;
	aabb box_b;
	a->bounding_box(0, &box_a);
	b->bounding_box(0, &box_b);
	return box_a.min().z < box_b.min().z;
}

bvh_node::bvh_node(
	const std::vector<std::shared_ptr<hittable>>& src_objects,
	size_t start, size_t end, double exposureTime
) {
	auto objects = src_objects; // Create a modifiable array of the source scene objects

	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare
		: (axis == 1) ? box_y_compare
		: box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = objects[start];
		right = 0;
	} else if (object_span == 2) {
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		} else {
			left = objects[start + 1];
			right = objects[start];
		}
	} else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = start + object_span / 2;
		left = std::make_shared<bvh_node>(objects, start, mid, exposureTime);
		right = std::make_shared<bvh_node>(objects, mid, end, exposureTime);
	}

	::aabb box_left, box_right;

	if (!left->bounding_box(exposureTime, &box_left)
		|| (right != 0 && !right->bounding_box(exposureTime, &box_right)))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	aabb = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	if (!aabb.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max, rec);
	if (right == 0)
		return hit_left;
	if (hit_left) {
		right->hit(r, t_min, rec->t, rec);
		return true;
	} else {
		return right->hit(r, t_min, t_max, rec);
	}
}

bool bvh_node::bounding_box(double exposureTime, ::aabb* output_box) const {
	*output_box = aabb;
	return true;
}
