// Termm--Fall 2022

#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
//#define RENDER_BOUNDING_VOLUMES

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	size_t normal1;
	size_t normal2;
	size_t normal3;


	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}

	Triangle( size_t pv1, size_t pv2, size_t pv3, size_t pv4, size_t pv5, size_t pv6 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
		, normal1( pv4 )
		, normal2( pv5 )
		, normal3( pv6 )
	{}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  virtual ~Mesh();
  bool hit(Ray ray, Intersection & intersection, float & ray_length) override;
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;
	std::vector<glm::vec3> m_normal;
	Primitive * bounding_sphere;
    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};
