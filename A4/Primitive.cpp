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
    if ( new_t < ray_length && new_t > 0) {
        ray_length = new_t;
        intersection.hit_point = ray.Get_origin() + new_t * ray.Get_direction();
        intersection.normal = intersection.hit_point - m_pos;
        //intersection.material = nonhiersphere.material
        return true;
    }
    return false;
}

NonhierBox::~NonhierBox() {}


bool triangleIntersection(Ray &ray, vec3 vertex0, vec3 vertex1, vec3 vertex2, float &res) {
	const float EPSILON = 0.0000001;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0; 

	h = glm::cross(ray.Get_direction(), edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1 / a;
	s = ray.Get_origin() - vertex0;
	u = f * (glm::dot(s, h));
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.Get_direction(), q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.

	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		res = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
    return false;
}

//make sure that such triangle exist!
bool trangleIntersection2(Ray ray, vec3 vertex1, vec3 vertex2, vec3 vertex3, float & t_max) {
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
        t_max = t;
        return true;
    }
    return false;
}

vec3 real_normal(vec3 direction, vec3 prev_normal) {
    if (dot(direction, prev_normal) > 0 ) {
        return -prev_normal;
    }
    return prev_normal;
}
bool NonhierBox::hit(Ray ray, Intersection & intersection, float & ray_length) {
    vec3 vertex1 = m_pos + vec3(0, m_size, m_size);
    vec3 vertex2 = m_pos + vec3(0, m_size, 0);
    vec3 vertex3 = m_pos + vec3(m_size, m_size, 0);
    vec3 vertex4 = m_pos + vec3(m_size, m_size, m_size);
    vec3 vertex5 = m_pos + vec3(0, 0, m_size);
    vec3 vertex6 = m_pos + vec3(0, 0, 0);
    vec3 vertex7 = m_pos + vec3(m_size, 0, 0);
    vec3 vertex8 = m_pos + vec3(m_size, 0, m_size);
    bool result = false;
   
    /*if ( triangleIntersection(ray, vertex1, vertex2, vertex4, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex2 - vertex1, vertex4 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex2, vertex3, vertex4, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex3 - vertex2, vertex4 - vertex2);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex1, vertex2, vertex6, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex2 - vertex1, vertex6 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex1, vertex5, vertex6, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex5 - vertex1, vertex6 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex2, vertex6, vertex7, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex6 - vertex2, vertex7 - vertex2);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex2, vertex3, vertex7, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex3 - vertex2, vertex7 - vertex2);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex1, vertex4, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex4 - vertex1, vertex8 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    
    if ( triangleIntersection(ray, vertex3, vertex4, vertex7, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex4 - vertex3, vertex7 - vertex3);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex8, vertex4, vertex7, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex4 - vertex8, vertex7 - vertex4);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex5, vertex6, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex8 - vertex5, vertex8 - vertex6);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }
    if ( triangleIntersection(ray, vertex6, vertex7, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex8 - vertex6, vertex8 - vertex7);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }*/
    if ( triangleIntersection(ray, vertex1, vertex5, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex5 - vertex1, vertex8 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        //std::cout << intersection.normal.x << " " << intersection.normal.y << " " << intersection.normal.z <<std::endl;
        result = true;
    }
    if (result) {
        //std::cout << "cube hit" << std::endl;
    }
    return result;
}
