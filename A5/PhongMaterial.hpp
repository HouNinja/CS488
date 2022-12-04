// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"
using namespace glm;

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double coefrefle, double coefrefra, double factor);
  virtual ~PhongMaterial();
  vec3 Get_kd();
  vec3 Get_ks();
  double Get_shininess();
  double coef_reflection();
  double coef_refraction();
  double refraction_factor();

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_coef_reflection;
  double m_coef_refraction;
  double m_refraction_factor;
};
