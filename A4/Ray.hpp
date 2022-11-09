#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"


class Ray {
    private:
    glm::vec3 origin;
    glm::vec3 direction;
    public:
    Ray(const glm::vec3& o, const glm::vec3& d);
    glm::vec3 Get_origin();
    glm::vec3 Get_direction();

};