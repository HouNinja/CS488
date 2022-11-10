#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"
#include "Image.hpp"

using namespace glm;

class Ray {
    private:
    glm::vec3 origin;
    glm::vec3 direction;
    public:
    Ray();
    Ray(const vec3& o, const vec3& d);
    vec3 Get_origin();
    vec3 Get_direction();
    void Set_origin(vec3 neworigin);
    void Set_direction(vec3 newdirection);
};