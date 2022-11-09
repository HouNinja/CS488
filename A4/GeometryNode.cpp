// Termm--Fall 2022

#include "GeometryNode.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

bool GeometryNode::hit(Ray ray, Intersection & intersection, float & ray_length) {
	Ray temp_ray;
	temp_ray.Set_origin(vec3(invtrans * vec4(ray.Get_origin(), 1.0f)));
	temp_ray.Set_direction(vec3(invtrans * vec4(ray.Get_direction(), 0.0f)));
	float min_length = ray_length;
	Intersection temp_intersection;
	for (SceneNode *child : children) {
		if (child->hit(temp_ray, temp_intersection, ray_length) && min_length > ray_length) {
			intersection = temp_intersection;
			return true;
		}
	}
	return false;
	return false;
}