// Termm--Fall 2022

#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Intersection.hpp"
#include <glm/glm.hpp>

using namespace glm;

Primitive::~Primitive() {}
bool Primitive::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return false;
}

Sphere::~Sphere() {}
bool Sphere::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return false;
}

Cube::~Cube() {}
bool Cube::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return false;
}

NonhierSphere::~NonhierSphere() {}

bool NonhierSphere::hit(Ray ray, Intersection & intersection, float & ray_length) {
    // If there exists a hit point: dot(origin + t * direction - m_mos) = radius ^ 2;
    vec3 temp = ray.Get_origin() - m_pos;
    float A = dot(ray.Get_direction(), ray.Get_direction());
    float B = 2 * dot(ray.Get_direction(), temp);
    float C = dot(temp, temp) - m_radius * m_radius;
    double roots[2];
    size_t n_root = quadraticRoots(A, B, C, roots);

    float new_t = ray_length;
    if ( n_root == 0 ) {
        return false;
    } else if ( n_root == 1 ) {
        new_t = roots[0];
    } else {
        new_t = std::min(roots[0], roots[1]);
    }
    if ( new_t < ray_length ) {
        ray_length = new_t;
        intersection.hit_point = ray.Get_origin() + new_t * ray.Get_direction();
        intersection.normal = intersection.hit_point - m_pos;
        //intersection.material = nonhiersphere.material
        return true;
    }
    return false;
}

NonhierBox::~NonhierBox() {}

bool NonhierBox::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return false;
}
