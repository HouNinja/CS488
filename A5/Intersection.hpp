#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

#ifndef TEXTURE
#include "Texture.hpp"
#endif

class Intersection {
    public:
    glm::vec3 hit_point;
    glm::vec3 normal;
    Material *material;
    bool is_textured = false;
    glm::vec3 texture_color;
    Texture * texture;
};