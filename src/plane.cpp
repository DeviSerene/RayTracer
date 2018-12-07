#include "plane.h"
#include <iostream>

Plane::Plane(glm::vec3 _planeCentre, glm::vec3 _planeNormal, glm::vec3 _material, glm::vec3 _spec)
{
	m_planeCentre = _planeCentre;
	m_planeNormal = _planeNormal;
	m_material = _material;
	m_specCol = _spec;
	m_reflectiveness = 0.5f;
	m_transparancy = 0;
	m_refractiveIndex = 0;
}

glm::vec3 Plane::Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD)
{
	return m_material * glm::vec3(155);

	float p = 10;

	glm::vec3 lightDirection = { 0.5, -1, 1 };
	lightDirection = _lightD;
	glm::vec3 lightColour = { 1, 1, 1 };

	glm::vec3 halfVector = (-_ray->direction + lightDirection) / (glm::length(-_ray->direction + lightDirection));

	float dotProduct = glm::dot(m_planeNormal, lightDirection);

	if (dotProduct < 0.00f)
	{
		dotProduct = 0.00f;

	}

	glm::vec3 ret = dotProduct * lightColour * m_material;
	ret += pow(glm::dot(halfVector, m_planeNormal), p) * lightColour * m_specCol;

	ret = glm::clamp(ret, 0.0f, 1.0f);
	//convert to 255
	ret.r = ret.r * 255.0f;
	ret.g = ret.g * 255.0f;
	ret.b = ret.b * 255.0f;

	return ret;

}

float Plane::TestRay(Ray* _ray)
{
	float distanceToHit = -1;

	float d = glm::dot(glm::normalize(_ray->direction), glm::normalize(m_planeNormal));

	if (d == 0)
		return -1;

	distanceToHit = (glm::dot((glm::normalize(m_planeCentre) - glm::normalize(_ray->origin)), glm::normalize(m_planeNormal))) / d;
	if (distanceToHit < 0)
		return -1;
	return distanceToHit;
}