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
		return m_objects[object]->Shade(_ray->origin + (_ray->direction * shortestDistance), _ray);
	}
	else
	{
		glm::vec3 black = { 30,30,30 };
		return black;
	}

}