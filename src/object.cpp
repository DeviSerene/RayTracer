#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

glm::vec3 Object::Shade(glm::vec3 _samplePoint, Ray* _ray, glm::vec3 _lightD)
{
	float red, green, blue;

	red = 1, green = 0, blue = 0;

	//convert to 255
	red = red * 255;
	green = green * 255;
	blue = blue * 255;

	return glm::vec3(red, green, blue);
}