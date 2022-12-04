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

Sphere::Sphere() {
    nonhiersphere = new NonhierSphere(vec3(0.0, 0.0, 0.0), 1.0);
}

Sphere::~Sphere() {
    delete nonhiersphere;
}

bool Sphere::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return nonhiersphere->hit(ray, intersection, ray_length);
}

Cube::Cube() {
    nonhiercube = new NonhierBox(vec3(0.0, 0.0, 0.0), 1.0);
}

Cube::~Cube() {
    delete nonhiercube;
}

bool Cube::hit(Ray ray, Intersection & intersection, float & ray_length) {
    return nonhiercube->hit(ray, intersection, ray_length);
}

NonhierSphere::~NonhierSphere() {}

double NonhierSphere::Get_radius() {
    return m_radius;
}

bool NonhierSphere::hit(Ray ray, Intersection & intersection, float & ray_length) {
    // If there exists a hit point: dot(origin + t * direction - m_mos) = radius ^ 2;
    const float EPSILON = 0.0000005;
    vec3 temp = ray.Get_origin() - m_pos;
    double A = dot(ray.Get_direction(), ray.Get_direction());
    double B = 2 * dot(ray.Get_direction(), temp);
    double C = dot(temp, temp) - m_radius * m_radius;
    double roots[2];
    size_t n_root = quadraticRoots(A, B, C, roots);

    float new_t = ray_length;
    if ( n_root == 0 ) {
        return false;
    } else if ( n_root == 1 ) {
        new_t = roots[0];
    } else {
        new_t = std::min(roots[0], roots[1]);
        /*if ( roots[0] >= 0 && roots[1] >=0 ) {
            new_t = std::min(roots[0], roots[1]);
        } else if ( roots[0] < 0 ) {
            new_t = roots[1];
        } else {
            new_t = roots[0];
        }*/
        //Possible Error: If one negative, one positive, the origin of ray is inside sphere;
    }
    //std::cout << new_t << std::endl;
    if ( new_t < ray_length && new_t > EPSILON) {
        ray_length = new_t - EPSILON;
        intersection.hit_point = ray.Get_origin() + (ray_length - 3 * EPSILON) * ray.Get_direction();
        intersection.normal = intersection.hit_point - m_pos;
        //intersection.material = nonhiersphere.material
        return true;
    }
    return false;
}

NonhierBox::~NonhierBox() {}


bool triangleIntersection(Ray &ray, vec3 vertex0, vec3 vertex1, vec3 vertex2, float &res) {
	const float EPSILON = 0.00001;
	glm::vec3 edge1, edge2, h, s, q;
	double a, f, u, v;
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
	if (t > EPSILON && t < res) // ray intersection
	{
		res = t - EPSILON;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
    return false;
}

//make sure that such triangle exist!
bool triangleIntersection2(Ray &ray, vec3 vertex1, vec3 vertex2, vec3 vertex3, float & t_max) {
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

    float t = (dot(vertex1, normal) - dot(ray.Get_origin(), normal)) / a;
    if ( t > t_max || t < EPSILON) {
        return false;
    }
    vec3 HitPoint = ray.Get_origin() + t * ray.Get_direction();
    //implement bycentric coordinates: P - A = beta * (B - A) + gamma * ( C - A )
    vec3 P_A = HitPoint - vertex1;
    vec3 B_A = vertex2 - vertex1;
    vec3 C_A = vertex3 - vertex1;
    float beta = (P_A.x * C_A.y - P_A.y * C_A.x) * 100000 / (100000 * (B_A.x * C_A.y - B_A.y * C_A.x));
    float gamma = (P_A.x * B_A.y - P_A.y * B_A.x) * 100000 / (100000 * (B_A.y * C_A.x - B_A.x * C_A.y));
    std::cout << beta << " " << gamma << std::endl;
    if ( beta + gamma <= 1 && beta + gamma >= 0 /*&& beta >= 0 && gamma >= 0*/) {
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
    float temp_length = ray_length;
    if ( triangleIntersection(ray, vertex1, vertex2, vertex4, ray_length)) {
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
    }
    if ( triangleIntersection(ray, vertex1, vertex5, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex5 - vertex1, vertex8 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        //std::cout << intersection.normal.x << " " << intersection.normal.y << " " << intersection.normal.z <<std::endl;
        result = true;
    }

     if ( triangleIntersection(ray, vertex1, vertex4, vertex8, ray_length) ) {
        intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
        vec3 prev_normal = cross(vertex4 - vertex1, vertex8 - vertex1);
        intersection.normal = real_normal(ray.Get_direction(), prev_normal);
        result = true;
    }

    

    if (result) {
        //std::cout << "cube hit" << std::endl;
    }
    return result;
}

Cylinder::Cylinder() {}
Cylinder::~Cylinder() {}

bool Cylinder::hit(Ray ray, Intersection & intersection, float & min_length) {
    const float EPSILON = 0.00001;
    bool result = false;
    float final_t = min_length;
    vec3 temp_norm;

    float A = ray.Get_direction().x * ray.Get_direction().x + ray.direction.z * ray.direction.z;
    float B = 2 * ( ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    float C = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
    double roots[2];
    int num_roots = quadraticRoots(A, B, C, roots);
    /*if ( num_roots > 0 ) {
         std::cout << "reach here" << std::endl;
    }*/
   
    if ( num_roots == 0 ) {
        return false;
    } else if ( num_roots == 1 ) {
        float t = roots[0];
        glm::vec3 destination = ray.origin + t * ray.direction;
        if ( destination.y >= -1 && destination.y <= 1 ) {
            temp_norm = normalize(vec3(destination.x, 0, destination.z));
            final_t = t;
            result = true;
        } 
    } else if ( num_roots == 2) {
        float t1 = roots[0];
        float t2 = roots[1];
        if ( t1 > t2 ) {
            std::swap(t1, t2);
        }

        glm::vec3 destination1 = ray.origin + t1 * ray.direction;
        glm::vec3 destination2 = ray.origin + t2 * ray.direction;
        if ( destination1.y < -1 ) {
            if ( destination2.y < -1 ) {
                return false;
            } else {
                float ratio = (destination2.y + 1) / (destination2.y - destination1.y);
                final_t = t1 + (t2 - t1) * (1 - ratio);
                result = true;
                temp_norm = vec3(0.0, -1.0, 0.0);
            }
        } else if ( destination1.y > 1 ) {
             if ( destination2.y > 1 ) {
                return false;
            } else {
                float ratio = (destination2.y - 1) / (destination2.y - destination1.y);
                final_t = t1 + (t2 - t1) * (1 - ratio);
                result = true;
                temp_norm = vec3(0.0, 1.0, 0.0);
            }
        } else {
            temp_norm = normalize(vec3(destination1.x, 0, destination1.z));
            final_t = t1;
            result = true;
        }
    }

    if ( result && final_t > EPSILON && final_t < min_length) {
        intersection.hit_point = ray.origin + final_t * ray.direction;
        intersection.normal = temp_norm;
        min_length = final_t - EPSILON;
        return true;
    }

    return false;
}


Cone::Cone() {}
Cone::~Cone() {}

bool Cone::hit(Ray ray, Intersection & intersection, float & min_length) {
    const float EPSILON = 0.00001;
    bool result = false;
    float final_t = min_length;
    vec3 temp_norm;

    float A = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z - ray.direction.y * ray.direction.y;
    float B = 2 * ( ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z - ray.origin.y * ray.direction.y);
    float C = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - ray.origin.y * ray.origin.y;
    double roots[2];
    int num_roots = quadraticRoots(A, B, C, roots);

    if ( num_roots == 0 ) {
        return false;
    } else if ( num_roots == 1 ) {
        float t = roots[0];
        glm::vec3 destination = ray.origin + t * ray.direction;
        if ( destination.y > 0 && destination.y <= 1 ) {
            vec3 radius(destination.x, 0 , destination.z);
            vec3 tangent = cross(radius, destination);
            temp_norm = normalize(cross(destination, tangent));
            if ( temp_norm.y > 0 ) {
                temp_norm = - temp_norm;
            }
            final_t = t;
            result = true;
        } 
    } else if ( num_roots == 2) {
        float t1 = roots[0] + EPSILON;
        float t2 = roots[1] + EPSILON;
        if ( t1 > t2 ) {
            std::swap(t1, t2);
        }

        glm::vec3 destination1 = ray.origin + t1 * ray.direction;
        glm::vec3 destination2 = ray.origin + t2 * ray.direction;
        if ( destination1.y <= 0 ) {
            if ( destination2.y <= 0 ) {
                return false;
            } else if ( destination2.y <= 1){
                vec3 radius(destination2.x, 0 , destination2.z);
                vec3 tangent = cross(radius, destination2);
                temp_norm = normalize(cross(destination2, tangent));
                if ( temp_norm.y > 0 ) {
                    temp_norm = - temp_norm;
                }
                final_t = t2;
                result = true;
            } else {
                return false;
            }
        } else if ( destination1.y <= 1 ) {
            if ( destination2.y > 0 ) {
                vec3 radius(destination1.x, 0 , destination1.z);
                vec3 tangent = cross(radius, destination1);
                temp_norm = normalize(cross(destination1, tangent));
                if ( temp_norm.y > 0 ) {
                    temp_norm = - temp_norm;
                }
                final_t = t1;
                result = true;
            } else {
                float new_t = (1 - ray.origin.y) / ray.direction.y;
                vec3 cap_intersection = ray.origin + new_t * ray.direction;
                if ( cap_intersection.x >= -1 && cap_intersection.x <= 1 && cap_intersection.z >= -1 && cap_intersection.z <= 1) {
                    final_t = new_t;
                    result = true;
                    temp_norm = vec3(0, 1, 0);
                }
            }

        } else {
           if ( destination2.y > 1 ) {
                return false;
            } else if ( destination2.y > 0 ){
                float new_t = (1 - ray.origin.y) / ray.direction.y;
                vec3 cap_intersection = ray.origin + new_t * ray.direction;
                if ( cap_intersection.x >= -1 && cap_intersection.x <= 1 && cap_intersection.z >= -1 && cap_intersection.z <= 1) {
                    final_t = new_t;
                    result = true;
                    temp_norm = vec3(0, 1, 0);
                }
            } else {
                return false;
            }
        }
    }
    if ( result && final_t > EPSILON && final_t < min_length ) {
        intersection.hit_point = ray.origin + final_t * ray.direction;
        intersection.normal = temp_norm;
        min_length = final_t;
        return true;
    }
    return false;
}