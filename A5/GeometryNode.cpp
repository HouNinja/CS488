// Termm--Fall 2022

#include "GeometryNode.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


#include <lodepng/lodepng.h>
#include <vector>
 std::vector<unsigned char> image;
    uint width;
    uint height;
	
    unsigned error = lodepng::decode(image, width, height, "smile.png");
vec3 bump_mapping(vec3 & normal, Ray ray, float x, float z) {
   
	//if (error) std::cout << "decoder error" << error << ":" << lodepng_error_text(error) << std::endl;
    int pixel_x = (x + 1) / 2 * (width - 1);
    int pixel_y = (-z + 1) / 2 * (height - 1);
    unsigned long position = pixel_y * width + pixel_x;
    position = position * 4;
	//std::cout << image.size() << " " << position << std::endl;
    vec3 color = vec3(image[position] / 255.0, image[position + 1] / 255.0, image[position + 2] / 255.0);

    vec3 N = normalize(normal);
	std::cout << to_string(N) << "    " << to_string(color) << std::endl;
    vec3 L = normalize(ray.Get_direction());
    vec3 U = cross(N,L);
    vec3 V = cross(U, N);
    normal = N;
    normal = normal + (1 - color.x) * 0.5 * U + (1 - color.y) * 0.5 * V + (1 - color.z) * 0.5 * N;
}


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
		
				//std::cout << "reach here" << std::endl;
				float x_difference =intersection.hit_point.x / (m_primitive->x_max - m_primitive->x_min);
				float y_difference =intersection.hit_point.y /  (m_primitive->y_max - m_primitive->y_min);
				//bump_mapping(intersection.normal, temp_ray, x_difference, y_difference);

		if ( m_texture != nullptr ) {
			if ( m_texture->texture_id == 1 ) {
				CheckerTexture * texture = static_cast<CheckerTexture *>(intersection.texture);
				intersection.texture_color = texture->get_color_3D(intersection.hit_point.x, intersection.hit_point.y, intersection.hit_point.z);
			} else if ( m_texture->texture_id == 2 ) {
				PerlinNoise * texture = static_cast<PerlinNoise *>(intersection.texture);
				float x_difference = m_primitive->x_max - m_primitive->x_min;
				float y_difference = m_primitive->y_max - m_primitive->y_min;
				float z_difference = m_primitive->z_max - m_primitive->z_min;
				intersection.texture_color = texture->get_color_3D(intersection.hit_point.x /x_difference, intersection.hit_point.y / y_difference, intersection.hit_point.z / z_difference);
			}
			
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