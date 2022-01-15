#include "aabb.h"

std::string aabb::to_string() const {
	return "aabb:" + minimum.to_string() + "-" + maximum.to_string();
}
