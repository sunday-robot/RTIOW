#pragma once

#include "vec3.h"
#include "color.h"

double random_double();
double random_double(double min, double max);
int random_int(int min, int max);
vec3 randomVec3(double min, double max);
vec3 random_in_unit_disk();
vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_in_hemisphere(const vec3& normal);
color randomColor();
color randomColor(double min, double max);
