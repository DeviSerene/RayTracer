#include "tracer.h"
#define MAX_ITERATIONS 20

glm::vec3 Tracer::RayTrace(Ray* _ray, int _iterations, int _ignore)
{
	if (_iterations >= MAX_ITERATIONS)
		return glm::vec3(0);

	m_lightPos = glm::vec3(1, 3, -1);

	float shortestDistance = -1;
	int object = -1;

	for (int i = 0; i < m_objects.size(); i++)
	{
		if (i == _ignore)
			continue;

		float test = m_objects[i]->TestRay(_ray); //returns distance to object, or -1 if it didn't connect

		if (test > 0) //if there are multiple objects, which object is closest?
		{
			if (shortestDistance > 0)
			{
				if (shortestDistance > test)
				{
					shortestDistance = test;
					object = i;
				}
			}
			else
			{
				shortestDistance = test;
				object = i;
			}
		}
	}

	glm::vec3 col = glm::vec3(0);
	if (shortestDistance > 0)
	{
		Ray shadowRay;
		//shadowRay.direction = lightPosition - pHit;
		shadowRay.origin = _ray->origin + (_ray->direction * shortestDistance);
		shadowRay.direction = glm::normalize(-m_lightPos - shadowRay.origin);
		col += m_objects[object]->Shade(shadowRay.origin, _ray, shadowRay.direction);
		col = glm::clamp(col, 0.0f, 255.0f);


		///REFLECTIVENESS
		if (m_objects[object]->GetReflectiveness() > 0)
		{
			bool reflected = false;
			int ReflObj = -1;
			float refDis = 0;
			shadowRay.direction = glm::reflect(_ray->direction, m_objects[object]->GetNormal(shadowRay.origin));
			for (int i = 0; i < m_objects.size(); i++)
			{
				if (i == _ignore || i == object)
					continue;
				refDis = m_objects[i]->TestRay(&shadowRay);
				if (refDis > 0)
				{
					reflected = true;
					ReflObj = i;
					break;
				}
			}
			if (reflected)
			{
				glm::vec3 reflectedSamplePoint = shadowRay.origin + (shadowRay.direction * refDis);
				glm::vec3 reflC = RayTrace(&shadowRay, _iterations + 1);
				//col += glm::vec3(m_objects[object]->GetReflectiveness()) * m_objects[ReflObj]->Shade(reflectedSamplePoint, &shadowRay, glm::normalize(-m_lightPos - reflectedSamplePoint));
				col += glm::vec3(m_objects[object]->GetReflectiveness()  *reflC);
				col = glm::clamp(col, 0.0f, 255.0f);
			}
			else
			{
				col += glm::vec3(-30);
				col = glm::clamp(col, 0.0f, 255.0f);
			}
		}

		shadowRay.direction = glm::normalize(-m_lightPos - shadowRay.origin);
		///TRANSPARANCY
		if (m_objects[object]->GetTransparancy() > 0)
		{
			bool refracted = false;
			int ReflObj = -1;
			float refDis = 0;
			shadowRay.direction = glm::refract(_ray->direction, m_objects[object]->GetNormal(shadowRay.origin), m_objects[object]->GetRI());
			for (int i = 0; i < m_objects.size(); i++)
			{
				if (i == _ignore || i == object)
					continue;
				refDis = m_objects[i]->TestRay(&shadowRay);
				if (refDis > 0)
				{
					refracted = true;
					ReflObj = i;
					break;
				}
			}
			if (refracted)
			{
				glm::vec3 reflectedSamplePoint = shadowRay.origin + (shadowRay.direction * refDis);
				glm::vec3 reflC = RayTrace(&shadowRay, _iterations + 1, object);
				//col += glm::vec3(m_objects[object]->GetReflectiveness()) * m_objects[ReflObj]->Shade(reflectedSamplePoint, &shadowRay, glm::normalize(-m_lightPos - reflectedSamplePoint));
				col += glm::vec3(m_objects[object]->GetTransparancy()  *reflC);
				col = glm::clamp(col, 0.0f, 255.0f);
			}
			else
			{
				//col += glm::vec3(-30);
				//col = glm::clamp(col, 0.0f, 255.0f);
			}
		}

		//SHADOWS
		bool inShadow = false;
		bool inSemiShadow = false;
		float semiShadowness = 0;
		shadowRay.origin = _ray->origin + (_ray->direction * shortestDistance);
		shadowRay.direction = glm::normalize(-m_lightPos - shadowRay.origin);
		for (int i = 0; i < m_objects.size(); i++)
		{
			if (i == _ignore || i == object)
				continue;
			if (m_objects[i]->TestRay(&shadowRay) > 0)
			{
				if (m_objects[i]->GetTransparancy() <= 0)
				{
					inShadow = true;
					break;
				}
				else
				{
					inSemiShadow = true;
					semiShadowness += 1 - m_objects[i]->GetTransparancy();
				}
			}
		}
		if (inShadow)
		{
			col = glm::clamp(col * glm::vec3(0.1f), 0.0f, 255.0f);
		}
		else if (inSemiShadow)
		{
			if (semiShadowness > 0.9f)semiShadowness = 0.9f;
			col = glm::clamp(col * glm::vec3(1 - semiShadowness), 0.0f, 255.0f);
		}

	}
	else
	{
		col = glm::vec3( 30,30,30 );
	}



	return col;
}