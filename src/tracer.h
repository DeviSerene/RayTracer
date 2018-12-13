#pragma once
#include "object.h"
#include "ray.h"
#include <vector>
#include <glm/glm.hpp>

///
/// This class deals with following the ray, to determine what colour to finally output to the screen
///
class Tracer
{
public:
	Tracer();
	~Tracer() { for (int i = 0; i < m_objects.size(); i++) { delete m_objects[i]; } m_objects.clear();}
	inline void AddObject(Object* _ob) { m_objects.push_back(_ob);}
	glm::vec3 RayTrace(Ray* _ray, int _iterations = 0, int _ignore = -1);
	inline std::vector<Object*> GetObjects() { return m_objects; }

	void RemoveObject() { delete m_objects.back(); m_objects.pop_back(); }

private:
	void DetermineClosestHit(Ray* _ray, int& _ignore, float& shortestDistance, int& object);

	void Reflection(glm::vec3& col, Ray* _ray, Ray& shadowRay, int& _ignore, int& _iterations, int& object);
	void Transparancy(glm::vec3& col, Ray* _ray, Ray& shadowRay, int& _ignore, int& _iterations, int& object);
	void Shade(glm::vec3& col, Ray* _ray, Ray& shadowRay, int& _ignore, float& shortestDistance, int& object);

	std::vector<Object*> m_objects;
	glm::vec3 m_lightPos;
};