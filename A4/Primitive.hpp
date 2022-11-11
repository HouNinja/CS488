// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersection.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool hit(Ray ray, Intersection & intersection, float & ray_length);
};

class Sphere : public Primitive {
  Primitive * nonhiersphere;
public:
  Sphere();
  virtual ~Sphere();
  virtual bool hit(Ray ray, Intersection & intersection, float & ray_length) override;
};

class Cube : public Primitive {
  Primitive * nonhiercube;
public:
  Cube();
  virtual ~Cube();
  virtual bool hit(Ray ray, Intersection & intersection, float & ray_length) override;
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius) {}
  virtual ~NonhierSphere();
  virtual bool hit(Ray ray, Intersection & intersection, float & ray_length) override;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size) {}
  virtual ~NonhierBox();
  virtual bool hit(Ray ray, Intersection & intersection, float & min_length) override;
private:
  glm::vec3 m_pos;
  double m_size;
};
