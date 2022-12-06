// Termm--Fall 2022

#include <glm/ext.hpp>

using namespace glm;

#include "A4.hpp"
#include "Ray.hpp"
#include "PhongMaterial.hpp"
#include <iostream>

//randf() generate a random number from -1 to 1;

float randf() {
	float x;
	x = rand() / RAND_MAX;
	int y = rand() % 2;
	if ( y ) {
		x = -x;
	}
	return x;
}

float rand_float() {
    float x;
    do {
        x = (float) rand() / (RAND_MAX);
    } while (x == 1);
    return x;
}

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

		if ( intersection.texture == nullptr) {
			//std::cout << " texture missing" << std::endl;
		}

		PhongMaterial * material = static_cast<PhongMaterial *>(intersection.material);
		
		/*if ( material->coef_reflection() != 0 ) {
			std::cout << material->coef_reflection() << " data has been read " << material->Get_shininess() << std::endl;
		}*/

		vec3 kd = material->Get_kd();
		vec3 ks = material->Get_ks();
		double shininess = material->Get_shininess();

		if ( intersection.texture != nullptr) {
			kd = intersection.texture_color;
			//std::cout << to_string(kd) << std::endl;
		}
		// Ambient light for all hits.
		final_color += ambient * kd;
		//std::cout << kd.x << " kd" << kd.y << " " << kd.z << std::endl;
		for (Light * light : lights) {
			//could potentially generate error here
			float soft_shadow_coef = 1.0f;
			double r, attenuation;

			Ray lightray(intersection.hit_point, light->position - intersection.hit_point);
			#ifndef ENABLE_SOFT_SHADOW
			Intersection light_intersection;
			float max_t = std::numeric_limits<float>::max();
			if ( root->hit(lightray, light_intersection, max_t) ) {
				// if intersect, then there's something blocking the ray from hitpoint to light source
				//std::cout << max_t << std::endl;
				continue;
			}
			#endif

			#ifdef ENABLE_SOFT_SHADOW
			int num_lights = 20;
			int num_blocked = 0;
			attenuation = 0;
			for(int i = 0; i < num_lights; ++i) {
				vec3 light_center = light->position;
				vec3 new_light = vec3(light_center.x + (rand_float() - 0.5f) * 30, 
										light_center.y + (rand_float() - 0.5f) * 30,
										light_center.z + (rand_float() - 0.5f) * 30);
				Ray new_lightray(intersection.hit_point, new_light - intersection.hit_point);
				Intersection light_intersection;
				float max_t = std::numeric_limits<float>::max();
				if ( root->hit(new_lightray, light_intersection, max_t) ) {
					num_blocked++;
				}
			}
			soft_shadow_coef = 1.0f * (num_lights - num_blocked) / num_lights;
			/*if ( soft_shadow_coef < 0.1f ) {
				std::cout << " reach here: " << num_blocked << std::endl;
			}*/
			#endif

			r = length(lightray.Get_direction());
			attenuation = 1.0 / (light->falloff[0] + light->falloff[1] * r + light->falloff[2] * r * r);
			
			vec3 L = normalize(lightray.Get_direction());
			vec3 N = normalize(intersection.normal);
			vec3 R = normalize(2 * dot(L, N) * N - L);
			vec3 V = normalize(eye - intersection.hit_point);

			//diffuse
			//vec3 temp = attenuation * dot(L, N) * kd * light->colour;
			//std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;
			if ( attenuation * dot(L, N) > EPSILON ) {
				final_color += soft_shadow_coef * attenuation * dot(L, N) * kd * light->colour;
			}
			//specular
			//std::cout << R.x << R.y << R.z<< V.x << V.y << V.z << std::endl;
			if ( attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) > EPSILON ) {
				final_color += soft_shadow_coef * attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) * ks * light->colour;
			}
			//final_color += attenuation * pow(glm::max(0.0f, dot(R,V)), shininess) * ks * light->colour;
			//std::cout << dot(R, V) << std::endl;
			//Code for mirrow reflection:
			if ( n_hits > 0 ) {
				//reflection
				vec3 reflected_direction = (2 * dot(V,N) * N - V) * 50;
				Ray reflected_ray = Ray(intersection.hit_point + EPSILON * N, reflected_direction);
				float reflection = material->coef_reflection();
				vec3 reflected = trace_ray(reflected_ray, root, eye, ambient, lights, n_hits - 1);

				if ( reflected.g != reflected.g) {
					std::cout << "reach here" << std::endl;
					vec3 temp = intersection.hit_point;
					std::cout << N.x << N.y << N.z << std::endl;
					std::cout << eye.x << eye.y << eye.z << std::endl;
					std::cout << temp.x << temp.y << temp.z << std::endl;
					std::cout << V.x << V.y << V.z << std::endl;
					std::cout << "reflected: " << reflected.r << " " << reflected.g << " " << reflected.b << std::endl;
					//std::cout << "sin1: " << sin_theta1 << "sin2 " << sin_theta2 << "hits" << n_hits << std::endl;
					//std::cout << "cos1: " << cosine_theta1 << "cos2 " << cos_theta2 << "hits" << n_hits << std::endl;
				}

				#ifdef ENABLE_GLOSSY_REFLECTION
				vec3 reflected_direction_unit = normalize(reflected_direction);
				vec3 unit_u = cross(reflected_direction_unit, N);
				vec3 unit_v = cross( unit_u, reflected_direction_unit);
				float glossy_radius = 0.2f;
				
				//std::cout << "reflected before "<< to_string(reflected) << std::endl;
				reflected = reflected / 9;
				for (int i = 0; i < 8; ++i) {
					
					float u = -glossy_radius / 2 + rand_float() * glossy_radius;
					float v = -glossy_radius / 2 + rand_float() * glossy_radius;
					vec3 glossy_direction = reflected_direction_unit + u * unit_u + v * unit_v;
					Ray glossy_reflected_ray(intersection.hit_point, glossy_direction);
					float cos = dot(normalize(glossy_direction), reflected_direction_unit);
					//std::cout << to_string(glossy_direction) << "     " << to_string(reflected_direction_unit) << std::endl;
					reflected += cos * trace_ray(glossy_reflected_ray, root, eye, ambient, lights, n_hits - 1) / 9;
				}
				//std::cout << "reflected after "<< to_string(reflected) << std::endl;
				#endif


				
				// Refraction
				//vec3 unit_d = normalize(Ray.Get_direction());
				double factor = material->refraction_factor();
				double cosine_theta1 = -dot(ray.Get_direction(), N) / length(ray.Get_direction()) / length(N);
				double sin_theta1 = sqrt(1 - pow(cosine_theta1, 2));
				double sin_theta2 = sin_theta1 / factor;
				double cos_theta2 = sqrt(1 - pow(sin_theta2, 2));

				/*if ( cosine_theta1 <= EPSILON || cosine_theta1 >= 1 - EPSILON ) {
					std::cout << "such point exist" << std::endl;
				}*/

				vec3 projection = -intersection.normal * cosine_theta1 * length(ray.Get_direction());
				vec3 difference = normalize(ray.Get_direction() - projection);
				vec3 refracted_direction = projection + (length(ray.Get_direction()) * cosine_theta1 / cos_theta2 * sin_theta2) * difference;
				Ray refracted_ray = Ray(intersection.hit_point  - 2 * EPSILON * N , refracted_direction);
				float refraction = material->coef_refraction();

				vec3 refracted = trace_ray(refracted_ray, root, eye, ambient, lights, n_hits - 1);
				//std::cout << "cos1: " << cosine_theta1 << "cos2 " << cos_theta2 << std::endl;
				//std::cout << "refracted: " <<refracted.r << " " << refracted.g << " " << refracted.b << std::endl;
				
				/*if ( cosine_theta1 <= EPSILON || cosine_theta1 >= 1 - EPSILON || cos_theta2 <= EPSILON || cos_theta2 >= 1 - EPSILON) {
					refracted = final_color;
				}*/

				if ( refracted.g != refracted.g ) {
					//std::cout << "refracted: " << refracted_direction.r << " " << refracted_direction.g << " " << refracted_direction.b << std::endl;
					//std::cout << "sin1: " << sin_theta1 << "sin2 " << sin_theta2 << "hits" << n_hits << std::endl;
					//std::cout << "cos1: " << cosine_theta1 << "cos2 " << cos_theta2 << "hits" << n_hits << std::endl;
				}
				
				final_color = final_color * (1 - reflection - refraction) + 
				reflected * reflection + refracted * refraction;
				
				//std::cout << "final: " << final_color.r << " " << final_color.g << " " << final_color.b << std::endl;
				//std::cout << -dot(ray.Get_direction(), N) << " " << length(ray.Get_direction()) << std::endl;
				

				/*final_color = final_color * (1 - reflection) + 
				trace_ray(reflected_ray, root, intersection.hit_point, ambient, lights, n_hits - 1) * reflection;*/
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
	srand(0);
 	for (uint y = 0; y < h; ++y) {
		std::cout << "y: " << y << std::endl;
		for (uint x = 0; x < w; ++x) {
			std::cout << "x: " << x << std::endl;
			const vec3 direction = Top_Left_corner + x * unit_x + y * unit_y;
			Ray ray = Ray(eye, direction);
			vec3 color = trace_ray(ray, root, eye, ambient, lights, 2);
			#ifdef ENABLE_ANTI_ALIASING
			//std::cout << "anti-aliasing enabled" << std::endl;
			const float threshhold = 0.000001;
			vec3 final_color = vec3(0.0, 0.0, 0.0);
			for (int i = -1; i <= 1; ++i) {
				for (int j = -1; j <= 1; ++j) {
					if ( i == 0 && j == 0 ) {
						final_color += color;
					} else {
						const vec3 temp_direction = Top_Left_corner + x * unit_x + y * unit_y + i * unit_x / 2.0 + j * unit_y / 2.0;
						Ray temp_ray = Ray(eye, temp_direction);
						final_color += trace_ray(temp_ray, root, eye, ambient, lights, 1);
					}
				}
			}
			final_color /= 9;
			/*if ( length(final_color - color) > threshhold ) {
				color = final_color;
			}*/
			color = final_color;
			
			#endif
			// Red: 
			image(x, y, 0) = (double)color.r;
			// Green: 
			image(x, y, 1) = (double)color.g;
			// Blue: 
			image(x, y, 2) = (double)color.b;
		}
	}

}
