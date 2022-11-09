#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"
#include "Image.hpp"


class Ray {
    private:
    glm::vec3 origin;
    glm::vec3 direction;
    public:
    Ray();
    Ray(glm::vec3 o, glm::vec3 d);
    glm::vec3 Get_origin();
    glm::vec3 Get_direction();
    void Set_origin(glm::vec3 neworigin);
    void Set_direction(glm::vec3 newdirection);
};