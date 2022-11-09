#include "Ray.hpp"

using namespace glm;

Ray::Ray() 
    : origin(vec3(0.0f, 0.0f, 0.0f)), direction(vec3(0.0f, 0.0f, 0.0f)) {}

Ray::Ray(vec3 o, vec3 d)
    : origin(o), direction(d) {}

vec3 Ray::Get_origin() {
    return origin;
}

vec3 Ray::Get_direction() {
    return direction;
}

void Ray::Set_origin(vec3 neworigin) {
    origin = neworigin;
}

void Ray::Set_direction(vec3 newdirection) {
    direction = newdirection;
}
