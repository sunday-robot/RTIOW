#include "xy_rect.h"

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	auto t = (k - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
		return false;

	auto x = r.origin().x() + t * r.direction().x();
	auto y = r.origin().y() + t * r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	auto outward_normal = vec3(0, 0, 1);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);

	return true;
}
