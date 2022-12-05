// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersection.hpp"

class Primitive {
public:
  float x_max = 1;
  float x_min = 0;
  float y_max = 1;
  float y_min = 0;
  float z_max = 1;
  float z_min = 0;

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
    : m_pos(pos), m_radius(radius) { 
    x_max = pos.x + radius;
    x_min = pos.x - radius;
    y_max = pos.y + radius;
    y_min = pos.y - radius;
    z_max = pos.z + radius;
    z_min = pos.z - radius; }
  virtual ~NonhierSphere();
  virtual bool hit(Ray ray, Intersection & intersection, float & ray_length) override;
  double Get_radius();

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size) {
      x_max = pos.x + size;
      x_min = pos.x;
      y_max = pos.y + size;
      y_min = pos.y;
      z_max = pos.z + size;
      z_min = pos.z;
    }
  virtual ~NonhierBox();
  virtual bool hit(Ray ray, Intersection & intersection, float & min_length) override;
private:
  glm::vec3 m_pos;
  double m_size;
};

class Cylinder : public Primitive {
public:
  Cylinder();
  /*NonhierCylinder(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height) {}*/
  virtual ~Cylinder();
  virtual bool hit(Ray ray, Intersection & intersection, float & min_length) override;
};

class Cone : public Primitive {
public:
  Cone();
  /*NonhierCylinder(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height) {}*/
  virtual ~Cone();
  virtual bool hit(Ray ray, Intersection & intersection, float & min_length) override;
};