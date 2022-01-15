#include "translate.h"

bool translate::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	ray moved_r(r.origin - offset, r.direction, r.time);
	if (!ptr->hit(moved_r, t_min, t_max, rec))
		return false;

	rec->p += offset;
	rec->set_face_normal(moved_r, rec->normal);

	return true;
}


aabb translate::bounding_box(double exposureTime) const {
	auto tmp = ptr->bounding_box(exposureTime);
	return aabb(tmp.minimum + offset, tmp.maximum + offset);
}
