// Termm--Fall 2022

#include <glm/ext.hpp>

using namespace glm;

#include "A4.hpp"

vec3 trace_ray(
	Ray * ray,
	SceneNode * root,
	const glm::vec3 & eye,
	const glm::vec3 & ambient,
	const std::list<Light *> & lights
) {
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
	vec3 Top_Left_corner = distance * unit_z - unit_x * (float)w / 2 - unit_y * (float)h / 2;

 	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			vec3 direction = Top_Left_corner + x * unit_x + y * unit_y;
			// Red: 
			image(x, y, 0) = (double)1.0;
			// Green: 
			image(x, y, 1) = (double)0.0;
			// Blue: 
			image(x, y, 2) = (double)1.0;
		}
	}

}
