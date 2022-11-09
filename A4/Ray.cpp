#include "Ray.hpp"
#include <glm/glm.hpp>

using namespace glm;

Ray::Ray(vec3 o, vec3 d)
    : origin(o), direction(d) {}

vec3 Ray::Get_origin() {
    return origin;
}

vec3 Ray::Get_direction() {
    return direction;
}