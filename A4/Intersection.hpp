#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

class Intersection {
    private:
    glm::vec3 hit_point;
    glm::vec3 normal;
    Material *material;
};