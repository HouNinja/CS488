// Termm--Fall 2022

#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Intersection.hpp"
#include <glm/glm.hpp>
#include <iostream>
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

//make sure that such triangle exist!
bool Ray_Triangle_Intersection(Ray ray, Intersection & intersection, float & t_max, 
vec3 vertex1, vec3 vertex2, vec3 vertex3) {
    const float EPSILON = 0.0000001;
    vec3 edge1 = vertex2 - vertex1;
    vec3 edge2 = vertex3 - vertex1;
    vec3 normal = cross(edge1, edge2);
    float a = dot(ray.Get_direction(), normal);
    if (a > dot(ray.Get_direction(), -normal)) { normal = -normal;}

    if (a < EPSILON && a > -EPSILON) {
        //if a == 0, then the ray is horizontal to the plane, Epsilon eliminate the horizontal rays
        return false;
    }

    float t = (dot(ray.Get_origin(), normal) - dot(vertex1, normal)) / a;
    if ( t > t_max ) {
        return false;
    }
    vec3 HitPoint = ray.Get_origin() + t * ray.Get_direction();
    //implement bycentric coordinates: P - A = beta * (B - A) + gamma * ( C - A )
    vec3 P_A = HitPoint - vertex1;
    vec3 B_A = vertex2 - vertex1;
    vec3 C_A = vertex3 - vertex1;
    float beta = (P_A.x * C_A.y - P_A.y * C_A.x) * 100000 / (100000 * (B_A.x * C_A.y - B_A.y * C_A.x));
    float gamma = (P_A.x * B_A.y - P_A.y * B_A.x) * 100000 / (100000 * (B_A.y * C_A.x - B_A.x * C_A.y));
    if ( beta + gamma <= 1 && beta + gamma >= 0 ) {
        intersection.hit_point = HitPoint;
        intersection.normal = normal;
        return true;
    }
    return false;
}

bool NonhierBox::hit(Ray ray, Intersection & intersection, float & ray_length) {
    vec3 vertex1 = m_pos + vec3(-m_size / 2, m_size / 2, m_size / 2);
    vec3 vertex2 = m_pos + vec3(-m_size / 2, m_size / 2, -m_size / 2);
    vec3 vertex3 = m_pos + vec3(m_size / 2, m_size / 2, -m_size / 2);
    vec3 vertex4 = m_pos + vec3(m_size / 2, m_size / 2, m_size / 2);
    vec3 vertex5 = m_pos + vec3(-m_size / 2, -m_size / 2, m_size / 2);
    vec3 vertex6 = m_pos + vec3(-m_size / 2, -m_size / 2, -m_size / 2);
    vec3 vertex7 = m_pos + vec3(m_size / 2, -m_size / 2, -m_size / 2);
    vec3 vertex8 = m_pos + vec3(m_size / 2, -m_size / 2, m_size / 2);
    bool result = false;
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex1, vertex2, vertex4)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex2, vertex3, vertex4)) {
        result =  true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex1, vertex2, vertex6)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex1, vertex5, vertex6)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex2, vertex6, vertex7)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex2, vertex3, vertex7)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex1, vertex4, vertex8)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex1, vertex5, vertex8)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex3, vertex4, vertex7)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex8, vertex4, vertex7)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex5, vertex6, vertex8)) {
        result = true;
    }
    if (Ray_Triangle_Intersection(ray, intersection, ray_length, vertex6, vertex7, vertex8)) {
        result = true;
    }
    if (result) {
        std::cout << "reach here" << std::endl;
    }
    return result;
}
