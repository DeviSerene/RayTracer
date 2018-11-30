#include "tracer.h"

glm::vec3 Tracer::RayTrace(Ray* _ray)
{
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

	if (shortestDistance > 0)
	{
		bool inShadow = false;
		Ray shadowRay;
		//shadowRay.direction = lightPosition - pHit;
		shadowRay.origin = _ray->origin + (_ray->direction * shortestDistance);
		shadowRay.direction = glm::vec3 ( 0, -1, 5 ) - shadowRay.origin;
		for (int i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i]->TestRay(&shadowRay) > 0 && i != object)
			{
				inShadow = true;
				break;
			}
		}
		if (inShadow)
		{
			return glm::vec3( 60,60,60 );
		}
		else
		{
			return m_objects[object]->Shade(shadowRay.origin, _ray);
		}
	}
	else
	{
		glm::vec3 black = { 30,30,30 };
		return black;
	}

}