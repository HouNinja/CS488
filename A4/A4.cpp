// Termm--Fall 2022

#include <glm/ext.hpp>

using namespace glm;

#include "A4.hpp"
#include "Ray.hpp"
#include "PhongMaterial.hpp"
#include <iostream>

vec3 trace_ray(
	Ray ray,
	SceneNode * root,
	const glm::vec3 & eye,
	const glm::vec3 & ambient,
	const std::list<Light *> & lights
) {
	Intersection intersection;
	float max = std::numeric_limits<float>::max();
	vec3 final_color = vec3(0.0f, 0.0f, 0.0f);
	if ( root->hit(ray, intersection, max) ) {
		PhongMaterial * material = static_cast<PhongMaterial *>(intersection.material);
		vec3 kd = material->Get_kd();
		vec3 ks = material->Get_ks();
		double shininess = material->Get_shininess();
		// Ambient light for all hits.
		final_color += ambient * kd;

		for (Light * light : lights) {
			//could potentially generate error here
			Ray lightray(light->position, intersection.hit_point - light->position);
			Intersection light_intersection;
			float max_t = 1.0f - 0.0001f;
			if ( root->hit(lightray, light_intersection, max_t) ) {
				// if t < 1, then light is blocked by something before reaching intersection hit point.
				continue;
			}

			double r = length(lightray.Get_direction());
			double attenuation = 1.0 / (light->falloff[0] + light->falloff[1] * r + light->falloff[2] * r * r);

			vec3 L = normalize(-lightray.Get_direction());
			vec3 N = normalize(intersection.normal);
			vec3 R = normalize(2 * dot(L, N) * N - L);
			vec3 V = normalize(eye - intersection.hit_point);

			//diffuse
			final_color += attenuation * dot(L, N) * kd * light->colour;
			//specular
			final_color += attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) * light->colour;

		}
		return final_color;
	}
	return vec3(0.0f, 0.0f, 0.0f);
}

void A4_Render(
		// What to render  
		SceneNode * root,

		// Image to write to, set to a given width and height  
		Image & image,

		// Viewing parameters  
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters  
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...  

  std::cout << "F22: Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();

	vec3 unit_z = normalize(view);
	vec3 unit_x = normalize(cross(view, up));
	vec3 unit_y = normalize(cross(unit_z, unit_x));
	float distance = h / 2 / glm::tan(glm::radians(fovy / 2));
	//distance = distance / 4;
	vec3 Top_Left_corner = distance * unit_z - unit_x * (float)w / 2 - unit_y * (float)h / 2;

 	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			const vec3 direction = Top_Left_corner + x * unit_x + y * unit_y;
			Ray ray = Ray(eye, direction);
			vec3 color = trace_ray(ray, root, eye, ambient, lights);
			// Red: 
			image(x, y, 0) = (double)color.r;
			// Green: 
			image(x, y, 1) = (double)color.g;
			// Blue: 
			image(x, y, 2) = (double)color.b;
		}
	}

}
