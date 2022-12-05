// Termm--Fall 2022

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

#include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

using namespace glm;

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;
	
	//std::string test = "test.txt";
	std::ifstream ifs( fname.c_str() );
	double x_max = std::numeric_limits<double>::min();
	double y_max = std::numeric_limits<double>::min();
	double z_max = std::numeric_limits<double>::min();
	double x_min = std::numeric_limits<double>::max();
	double y_min = std::numeric_limits<double>::max();
	double z_min = std::numeric_limits<double>::max();
	//double radius = 0;

	while( ifs >> code ) {
		
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			x_max = std::max(vx, x_max);
			x_min = std::min(vx, x_min);
			y_max = std::max(vy, y_max);
			y_min = std::min(vy, y_min);
			z_max = std::max(vz, z_max);
			z_min = std::min(vz, z_min);
			//radius = std::max()
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
	
	//vec3 center = vec3((x_max + x_min) / 2, (y_max + y_min) / 2, (z_max + z_min) / 2);
	double radius = sqrt(pow((x_max - x_min) / 2, 2) + pow((y_max - y_min) / 2, 2) + pow((z_max - z_min) / 2, 2));
	radius *= 1.000001;

	//std::cout << (x_max + x_min) / 2 <<" " << (y_max + y_min) / 2 << " " << (z_max + z_min) / 2 << std::endl;
	std::cout << radius << std::endl;
	if ( radius > 0 ) {
		bounding_sphere = new NonhierSphere(vec3((x_max + x_min) / 2, (y_max + y_min) / 2, (z_max + z_min) / 2), radius);
	}
	
}

Mesh::~Mesh() {
	if ( bounding_sphere != nullptr ) {
		delete bounding_sphere;
	}
}
std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}


bool t_i(Ray &ray, vec3 vertex0, vec3 vertex1, vec3 vertex2, float &res) {
	const float EPSILON = 0.01;
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
		res = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
    return false;
}

vec3 r_n(vec3 direction, vec3 prev_normal) {
    if (dot(direction, prev_normal) > 0 ) {
        return -prev_normal;
    }
    return prev_normal;
}

bool Mesh::hit(Ray ray, Intersection & intersection, float & ray_length) {
	bool result = false;
	NonhierSphere * address = static_cast<NonhierSphere *>(bounding_sphere);
	float diameter_t = address->Get_radius() / length(ray.Get_direction());
	Ray temp_ray = Ray(ray.Get_origin() - (diameter_t + 1) * ray.Get_direction(), ray.Get_direction());
	Intersection temp_intersection;
	float max = std::numeric_limits<float>::max();
	if ( !bounding_sphere->hit(temp_ray, temp_intersection, max) ) {
		return false;
	}
	
#ifdef RENDER_BOUNDING_VOLUMES
	//std::cout << "the t1 value is " << max << std::endl;
	if ( bounding_sphere->hit(ray, intersection, ray_length) ) {
		//std::cout << "the t2 value is " << max << std::endl;
		//intersection = temp2_intersection;
		//ray_length = max;
		return true;
	}
#endif
	
	for (int i = 0; i < m_faces.size(); ++i) {
		Triangle triangle = m_faces[i];
		if ( t_i(ray, m_vertices[triangle.v1], m_vertices[triangle.v2], m_vertices[triangle.v3], ray_length) ) {
			intersection.hit_point = ray.Get_origin() + ray_length * ray.Get_direction();
			vec3 prev_normal = cross(m_vertices[triangle.v2] - m_vertices[triangle.v1], m_vertices[triangle.v3] - m_vertices[triangle.v1]);
			intersection.normal = r_n(ray.Get_direction(), prev_normal);
			result = true;
			//std::cout << "it reaches here" << std::endl;
		}

	}
	return result;
}
