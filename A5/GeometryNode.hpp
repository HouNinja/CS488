// Termm--Fall 2022

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

#ifndef TEXTURE
#include "Texture.hpp"
#endif

#include "Ray.hpp"
#include "Intersection.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr, Texture * tex = nullptr );

	void setMaterial( Material *material );
	void setTexture( Texture *texture);

	Material *m_material;
	Texture *m_texture;
	Primitive *m_primitive;

	virtual bool hit(Ray ray, Intersection & intersection, float & ray_t) override;
};
