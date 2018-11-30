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
	float GetReflectiveness() { return m_reflectiveness; }

protected:
	glm::mat4 m_modelMatrix;
	float m_reflectiveness;
};