#pragma once
#include "object.h"

///
/// 
///
class Sphere : public Object
{
public:
	Sphere(){}
	Sphere(glm::vec3 _sphereCentre, float _radius, glm::vec3 _material, glm::vec3 _spec, float _reflectiveness, float _transparancy, float _refraction);
	~Sphere() {}
	glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD);
	void Translate(glm::vec3 _by);
	virtual float TestRay(Ray* _ray);
	glm::vec3 GetNormal(glm::vec3 _samplePoint) {return glm::normalize(_samplePoint - m_sphereCentre);}
private:
	glm::vec3 m_sphereCentre;
	float m_radius;
	float m_roughness;
};