#include "random_utils.h"

#if false
// 下の乱数生成器も、マルチコア非対応だった。生成される画像の品質が向上するということもなかった。
#include <random>
static std::mt19937 engine;
double random_double() {
	// Returns a random real in [0,1).
	return engine() / (UINT32_MAX + 1.0);
}
#else
double random_double() {
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}
#endif

double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_double();
}

int random_int(int min, int max) {
	// Returns a random integer in [min,max].
	return rand() % (max + 1 - min) + min;
}

vec3 randomVec3(double min, double max) {
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_in_unit_sphere() {
	while (true) {
		auto p = randomVec3(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

color randomColor() {
	return color(random_double(), random_double(), random_double());
}

color randomColor(double min, double max) {
	return color(random_double(min, max), random_double(min, max), random_double(min, max));
}
