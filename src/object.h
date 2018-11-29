#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

class Object
{
public:
	Object();
	~Object();

	virtual glm::vec3 Shade(glm::vec3 _samplePoint, Ray* _ray);
	virtual float TestRay(Ray* _ray) { return 0; }

protected:
	glm::mat4 m_modelMatrix;
};