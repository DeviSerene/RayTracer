#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

class Camera
{
public:
	Camera(float _w, float _h);
	~Camera();

	Ray SpawnRay(int _x, int _y);
	void Move(glm::vec3 _movement);
	glm::vec3 GetPos() { return m_position; }
private:
	float m_width, m_height;
	glm::mat4 m_projectionMatrix, m_viewMatrix;
	glm::mat4 m_invPM, m_invVM;
	glm::vec3 m_position;

};