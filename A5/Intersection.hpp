#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

class Intersection {
    public:
    glm::vec3 hit_point;
    glm::vec3 normal;
    Material *material;
};