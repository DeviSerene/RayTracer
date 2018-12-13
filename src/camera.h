#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

///
/// The 'Camera' deals with creating rays, based on the screen size, in order to determine what pixel colour to draw at that specific coordinate.
/// The camera has a projection and view matrix; the user is able to move the camera.
///
class Camera
{
public:
	Camera(float _w, float _h);
	~Camera();

	///Create a Ray from the specific coordinate
	Ray SpawnRay(int _x, int _y);
	///Move the Camera by an amount
	void Move(glm::vec3 _movement);
	///Get the current position of the camera;
	inline glm::vec3 GetPos() { return m_position; }
private:
	///width and height of the screen
	float m_width, m_height;
	///The matrices
	glm::mat4 m_projectionMatrix, m_viewMatrix;
	glm::mat4 m_invPM, m_invVM;
	///The position of the camera in world space
	glm::vec3 m_position;

};