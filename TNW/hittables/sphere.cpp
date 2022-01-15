#include "sphere.h"

aabb sphere::bounding_box(double exposureTime) const {
	return aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	vec3 oc = r.origin - center;
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
	vec3 outward_normal = (p - center) / radius;

	rec->t = root;
	rec->p = p;
	rec->set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, &rec->u, &rec->v);
	rec->mat_ptr = mat_ptr;

	return true;
}

void sphere::print(std::string indent) const
{
	std::cout << indent << "(sphere)" << std::endl;
}
