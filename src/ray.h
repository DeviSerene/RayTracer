#pragma once
#include <glm/glm.hpp>

/// A ray contains two things: an origin and a direction   o---->
struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};