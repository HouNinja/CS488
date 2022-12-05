// Termm--Fall 2022

#include "GeometryNode.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat, Texture *tex )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
	, m_texture( tex )
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

void GeometryNode::setTexture( Texture *tex )
{
	m_texture = tex;
}

bool GeometryNode::hit(Ray ray, Intersection & intersection, float & ray_t) {
	Ray temp_ray;
	temp_ray.Set_origin(vec3(invtrans * vec4(ray.Get_origin(), 1.0f)));
	temp_ray.Set_direction(vec3(invtrans * vec4(ray.Get_direction(), 0.0f)));

	Intersection temp_intersection;
	float min_length = ray_t;
	bool result = false;
	for (SceneNode *child : children) {
		if ( child->hit(temp_ray, temp_intersection, min_length) ) {
			intersection = temp_intersection;
			ray_t = min_length;
			result = true;
		}
	}
	if ( m_primitive->hit(temp_ray, intersection, ray_t) ) {
		//std::cout << " reach here"<< std::endl
		intersection.material = m_material;
		intersection.texture = m_texture;
		if ( m_texture != nullptr ) {
			CheckerTexture * texture = static_cast<CheckerTexture *>(intersection.texture);
			//kd = intersection.texture_color;
			intersection.texture_color = texture->get_color_3D(intersection.hit_point.x, intersection.hit_point.y, intersection.hit_point.z);
		}
		result = true;
	}
	if ( result ) {
		// if it's false and intersection has a value(from another subtree), without the if, 
		intersection.hit_point = vec3(trans * vec4(intersection.hit_point, 1.0f));
		intersection.normal = transpose(mat3(invtrans)) * intersection.normal;
	}
	return result;
}