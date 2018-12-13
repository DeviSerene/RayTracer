#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

///
/// Object is a virtual class to which all kinds of objects should inherit from (such as Sphere or Plane)
/// All objects have some values such as colour, reflectivity and transparancy, etc
///
class Object
{
public:
	Object();
	~Object();
	///To determine what colour to display, we need a light source too
	virtual glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD);
	///We always need to determine if the ray hits this object
	virtual float TestRay(Ray* _ray) { return 0; }
	///We need a function to move the object
	virtual void Translate(glm::vec3 _by){}
	///The normal is required for reflectivity
	virtual glm::vec3 GetNormal(glm::vec3 _samplePoint) { return glm::vec3(0); }
	///Getters
	inline float GetReflectiveness() { return m_reflectiveness; }
	inline float GetTransparancy() { return m_transparancy; }
	inline float GetRI() { return m_refractiveIndex; }

protected:
	glm::mat4 m_modelMatrix;
	float m_reflectiveness;
	float m_transparancy;
	float m_refractiveIndex;
	glm::vec3 m_material;
	glm::vec3 m_specCol;
};