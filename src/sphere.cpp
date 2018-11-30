#include "sphere.h"
#include <iostream>

Sphere::Sphere(glm::vec3 _sphereCentre, float _radius, glm::vec3 _material, glm::vec3 _spec)
{
	m_sphereCentre = _sphereCentre;
	m_radius = _radius;
	m_material = _material;
	m_specCol = _spec;
	m_reflectiveness = 0;
}

void Sphere::Translate(glm::vec3 _by)
{
	m_sphereCentre += _by;
}

glm::vec3 Sphere::Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD)
{
	float p = 10;

	glm::vec3 normal = glm::normalize(_samplePoint - m_sphereCentre);
	glm::vec3 lightDirection = { 0.5, -1, 1 };
	lightDirection = _lightD;
	glm::vec3 lightColour = { 1, 1, 1 };

	glm::vec3 halfVector = (-_ray->direction + lightDirection) / (glm::length(-_ray->direction + lightDirection));

	float dotProduct = glm::dot(normal, lightDirection);

	if (dotProduct < 0.0f)
	{
		dotProduct = 0.0f;

	}

	glm::vec3 ret = dotProduct * lightColour * m_material;
	ret += pow(glm::dot(halfVector, normal), p) * lightColour * m_specCol;

	ret = glm::clamp(ret, 0.0f, 1.0f);
	//convert to 255
	ret.r = ret.r * 255.0f;
	ret.g = ret.g * 255.0f;
	ret.b = ret.b * 255.0f;

	return ret;
}

float Sphere::TestRay(Ray* _ray)
{
	float distanceToHit = -1;
	glm::vec3 PA = m_sphereCentre - _ray->origin;

	if (glm::length(PA) < m_radius)
	{
		return -1;
	}


	//std::cout << "PA: " << PA.x << "," << PA.y << "," << PA.z << std::endl;

	float dotProduct = glm::dot(PA, _ray->direction);

	if (dotProduct > 0) //intersection is in front of the camera
	{
		// P-a-((P-a).n)n
		float distanceToSphere = glm::length(PA - (dotProduct * _ray->direction));
		//std::cout << "dotProduct: " << dotProduct << std::endl;

		if (distanceToSphere <= m_radius)
		{
			float x = std::sqrt((m_radius * m_radius) - (distanceToSphere * distanceToSphere));
			distanceToHit = dotProduct - x;
		}
	}
	return distanceToHit;
}