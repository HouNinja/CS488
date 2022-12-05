// Termm--Fall 2022

#include "PhongMaterial.hpp"
#include <glm/glm.hpp>

using namespace glm;


PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_coef_reflection(0.0)
	, m_coef_refraction(0.0)
	, m_refraction_factor(0.0)
{}

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double coefrefle, double coefrefra, double factor)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_coef_reflection(coefrefle)
	, m_coef_refraction(coefrefra)
	, m_refraction_factor(factor)
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

double PhongMaterial::coef_reflection() {
	return m_coef_reflection;
}

double PhongMaterial::coef_refraction() {
	return m_coef_refraction;
}

double PhongMaterial::refraction_factor() {
	return m_refraction_factor;
}

void PhongMaterial::set_kd(glm::vec3 new_kd) {
	m_kd = new_kd;
	return;
}