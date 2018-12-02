#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

class Object
{
public:
	Object();
	~Object();

	virtual glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD);
	virtual float TestRay(Ray* _ray) { return 0; }
	virtual void Translate(glm::vec3 _by){}
	virtual glm::vec3 GetNormal(glm::vec3 _samplePoint) { return glm::vec3(0); }
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