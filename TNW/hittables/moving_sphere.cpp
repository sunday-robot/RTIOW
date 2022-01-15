#include "moving_sphere.h"

#include "../surrounding_box.h"

vec3 moving_sphere::centerAt(double time) const {
	return center + velocity * time;
}

aabb moving_sphere::bounding_box(double exposureTime) const {
	auto c0 = centerAt(-exposureTime / 2);
	auto c1 = centerAt(exposureTime / 2);
	aabb box0(
		c0 - vec3(radius, radius, radius),
		c0 + vec3(radius, radius, radius));
	aabb box1(
		c1 - vec3(radius, radius, radius),
		c1 + vec3(radius, radius, radius));
	return surrounding_box(box0, box1);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	vec3 oc = r.origin - centerAt(r.time);
	auto a = r.direction.length_squared();
	auto half_b = dot(oc, r.direction);
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	auto p = r.at(root);
	vec3 outward_normal = (p - centerAt(r.time)) / radius;

	rec->t = root;
	rec->p = p;
	rec->set_face_normal(r, outward_normal);
	rec->mat_ptr = mat_ptr;

	return true;
}
