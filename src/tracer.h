#pragma once
#include "object.h"
#include "ray.h"
#include <vector>
#include <glm/glm.hpp>

class Tracer
{
public:
	Tracer() {}
	~Tracer() { for (int i = 0; i < m_objects.size(); i++) { delete m_objects[i]; m_objects.clear(); } }
	void AddObject(Object* _ob) { m_objects.push_back(_ob);}
	glm::vec3 RayTrace(Ray* _ray);

private:
	std::vector<Object*> m_objects;

};