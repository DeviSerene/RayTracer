#pragma once
#include "object.h"

class Sphere : public Object
{
public:
	Sphere(){}
	Sphere(glm::vec3 _sphereCentre, float _radius, glm::vec3 _material, glm::vec3 _spec);
	~Sphere() {}
	glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray);
	virtual float TestRay(Ray* _ray);
private:
	glm::vec3 m_sphereCentre;
	float m_radius;
	glm::vec3 m_material;
	glm::vec3 m_specCol;
};