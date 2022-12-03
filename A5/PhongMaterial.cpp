// Termm--Fall 2022

#include "PhongMaterial.hpp"
#include <glm/glm.hpp>

using namespace glm;


PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}

vec3 PhongMaterial::Get_kd() {
	return m_kd;
}

vec3 PhongMaterial::Get_ks() {
	return m_ks;
}

double PhongMaterial::Get_shininess() {
	return m_shininess;
}