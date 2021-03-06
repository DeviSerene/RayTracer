#pragma once
#include "object.h"

///
/// A plane is an object, that is almost like a sheet of paper that goes on forever 
///
class Plane : public Object
{
public:
	Plane(){}
	Plane(glm::vec3 _planeCentre, glm::vec3 _planeNormal, glm::vec3 _material, glm::vec3 _spec);
	~Plane() {}

	glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD);
	virtual float TestRay(Ray* _ray);
	glm::vec3 GetNormal(glm::vec3 _samplePoint) { return m_planeNormal; }
private:
	glm::vec3 m_planeCentre;
	glm::vec3 m_planeNormal;

};