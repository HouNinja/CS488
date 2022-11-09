// Termm--Fall 2022

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	Material *m_material;
	Primitive *m_primitive;

	virtual bool hit(Ray ray, Intersection & intersection, float & ray_length) override;
};
