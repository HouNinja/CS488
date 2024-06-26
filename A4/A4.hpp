// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"

glm::vec3 trace_ray(
	Ray * ray,
	SceneNode * root,
	const glm::vec3 & eye,  
	const glm::vec3 & ambient,
	const std::list<Light *> & lights,
	int n_hits
);

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
);
