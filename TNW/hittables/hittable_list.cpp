#include "hittable_list.h"
#include "../surrounding_box.h"

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record* rec) const {
	hit_record temp_rec;
	auto hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, &temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}

	return hit_anything;
}

aabb hittable_list::bounding_box(double exposureTime) const {
	auto output_box = objects[0]->bounding_box(exposureTime);
	for (auto i = 1; i < objects.size(); i++) {
		auto temp_box = objects[i]->bounding_box(exposureTime);
		output_box = surrounding_box(output_box, temp_box);
	}
	return output_box;
}

void hittable_list::print(std::string indent) const
{
	std::cout << indent << "(hittable_list)" << std::endl;
}
