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
	const std::list<Light *> & lights,
	int n_hits
) {
	const float EPSILON = 0.000001;
	Intersection intersection;
	float max = std::numeric_limits<float>::max();
	vec3 final_color = vec3(0.0f, 0.0f, 0.0f);
	if ( root->hit(ray, intersection, max) ) {
		if ( intersection.material == nullptr) {
			std::cout << " material missing" << std::endl;
		}
		PhongMaterial * material = static_cast<PhongMaterial *>(intersection.material);
		vec3 kd = material->Get_kd();
		vec3 ks = material->Get_ks();
		double shininess = material->Get_shininess();
		// Ambient light for all hits.
		final_color += ambient * kd;
		//std::cout << kd.x << " kd" << kd.y << " " << kd.z << std::endl;
		for (Light * light : lights) {
			//could potentially generate error here
			Ray lightray(intersection.hit_point, light->position - intersection.hit_point);
			Intersection light_intersection;
			float max_t = std::numeric_limits<float>::max();
			if ( root->hit(lightray, light_intersection, max_t) ) {
				// if intersect, then there's something blocking the ray from hitpoint to light source
				//std::cout << max_t << std::endl;
				continue;
			}

			double r = length(lightray.Get_direction());
			double attenuation = 1.0 / (light->falloff[0] + light->falloff[1] * r + light->falloff[2] * r * r);
			
			vec3 L = normalize(lightray.Get_direction());
			vec3 N = normalize(intersection.normal);
			vec3 R = normalize(2 * dot(L, N) * N - L);
			vec3 V = normalize(eye - intersection.hit_point);

			//diffuse
			//vec3 temp = attenuation * dot(L, N) * kd * light->colour;
			//std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;
			if ( attenuation * dot(L, N) > EPSILON ) {
				final_color += attenuation * dot(L, N) * kd * light->colour;
			}
			//specular
			if ( attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) > EPSILON ) {
				final_color += attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) * ks * light->colour;
			}
			//final_color += attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) * ks * light->colour;
			//std::cout << dot(R, V) << std::endl;
			//Code for mirrow reflection:
			if ( n_hits > 0 ) {
				vec3 reflected_direction = (2 * dot(V,N) * N - V) * 50;
				Ray reflected_ray = Ray(intersection.hit_point, reflected_direction);
				float cofr = 0.15;
				final_color = final_color * (1 - cofr) + trace_ray(reflected_ray, root, intersection.hit_point, ambient, lights, n_hits - 1) *cofr;
			}

		}
	} else {
		vec3 unit = normalize(ray.Get_direction());
		final_color += unit.x * vec3(0.3,0.3,0.9) + (1 - unit.x) * vec3 (0.6, 0.6, 0.6);
		final_color += unit.y * vec3(0.3,0.3,0.9) + (1 - unit.y) * vec3 (0.6, 0.6, 0.6);
		final_color /= 2;
		//final_color = vec3(0.5, 0.5, 0.5);
	}
	return final_color;
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
			vec3 color = trace_ray(ray, root, eye, ambient, lights, 3);
			// Red: 
			image(x, y, 0) = (double)color.r;
			// Green: 
			image(x, y, 1) = (double)color.g;
			// Blue: 
			image(x, y, 2) = (double)color.b;
		}
	}

}
