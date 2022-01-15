#include "vec3.h"
#include <string>

std::string vec3::to_string() const
{
    std::string r;
    r += "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    return r;
}
