#include "tracer.h"

glm::vec3 Tracer::RayTrace(Ray* _ray)
{
	m_lightPos = glm::vec3(1, 10, 0);

	float shortestDistance = -1;
	int object = -1;

	for (int i = 0; i < m_objects.size(); i++)
	{
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
		bool inShadow = false;
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
				refDis = m_objects[i]->TestRay(&shadowRay);
				if (i != object &&  refDis > 0)
				{
					reflected = true;
					ReflObj = i;
					break;
				}
			}
			if (reflected)
			{
				glm::vec3 reflectedSamplePoint = shadowRay.origin + (shadowRay.direction * refDis);
				col += glm::vec3(m_objects[object]->GetReflectiveness()) * m_objects[ReflObj]->Shade(reflectedSamplePoint, &shadowRay, glm::normalize(-m_lightPos - reflectedSamplePoint));
				col = glm::clamp(col, 0.0f, 255.0f);
			}
			else
			{
				col += glm::vec3(-30);
				col = glm::clamp(col, 0.0f, 255.0f);
			}
		}

		//SHADOWS
		shadowRay.origin = _ray->origin + (_ray->direction * shortestDistance);
		shadowRay.direction = glm::normalize(-m_lightPos - shadowRay.origin);
		for (int i = 0; i < m_objects.size(); i++)
		{
			if (i != object && m_objects[i]->TestRay(&shadowRay) > 0)
			{
				inShadow = true;
				break;
			}
		}
		if (inShadow)
		{
			col = glm::clamp(col * glm::vec3(0.1f), 0.0f, 255.0f);
		}
		else
		{
		}

	}
	else
	{
		col = glm::vec3( 30,30,30 );
	}



	return col;
}