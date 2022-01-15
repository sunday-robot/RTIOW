#include "rotate_y.h"
#include <numbers>
#include <cmath>
#include "../degrees_to_radians.h"

static const double infinity = std::numeric_limits<double>::infinity();

rotate_y::rotate_y(std::shared_ptr<hittable> p, double angle) : ptr(p) {
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	bbox = ptr->bounding_box(1);

	vec3 min(infinity, infinity, infinity);
	vec3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i * bbox.max().x + (1 - i) * bbox.min().x;
				auto y = j * bbox.max().y + (1 - j) * bbox.min().y;
				auto z = k * bbox.max().z + (1 - k) * bbox.min().z;

				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z;

				vec3 tester(newx, y, newz);

				{
					min.x = fmin(min.x, tester.x);
					max.x = fmax(max.x, tester.x);
				}
				{
					min.y = fmin(min.y, tester.y);
					max.y = fmax(max.y, tester.y);
				}
				{
					min.z = fmin(min.z, tester.z);
					max.z = fmax(max.z, tester.z);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	auto origin = r.origin;
	auto direction = r.direction;

	origin.x = cos_theta * r.origin.x - sin_theta * r.origin.z;
	origin.z = sin_theta * r.origin.x + cos_theta * r.origin.z;

	direction.x = cos_theta * r.direction.x - sin_theta * r.direction.z;
	direction.z = sin_theta * r.direction.x + cos_theta * r.direction.z;

	ray rotated_r(origin, direction, r.time);

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	auto normal = rec->normal;

	auto px = cos_theta * rec->p.x + sin_theta * rec->p.z;
	auto pz = -sin_theta * rec->p.x + cos_theta * rec->p.z;

	normal.x = cos_theta * rec->normal.x + sin_theta * rec->normal.z;
	normal.z = -sin_theta * rec->normal.x + cos_theta * rec->normal.z;

	rec->p.x = px;
	rec->p.z = pz;
	rec->set_face_normal(rotated_r, normal);

	return true;
}

void rotate_y::print(std::string indent) const
{
	std::cout << indent << "(rotate_y)" << std::endl;
}
