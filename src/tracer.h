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
	inline void AddObject(Object* _ob) { m_objects.push_back(_ob);}
	glm::vec3 RayTrace(Ray* _ray, int _iterations = 0, int _ignore = -1);
	inline std::vector<Object*> GetObjects() { return m_objects; }

private:
	std::vector<Object*> m_objects;
	glm::vec3 m_lightPos;
};