#include "camera.h"

#define PI 3.14159265358979f

Camera::Camera(float _w, float _h)
{
	m_width = _w;
	m_height = _h;

	m_projectionMatrix = glm::perspective(30.0f * PI / 180.0f, m_width / m_height, 0.1f, 100.0f);
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5.0f));

	m_invPM = glm::inverse(m_projectionMatrix);
	m_invVM = glm::inverse(m_viewMatrix);
}

Camera::~Camera()
{

}

Ray Camera::SpawnRay(int _x, int _y)
{
	Ray ret;// = new Ray();
	glm::vec4 nearPoint, farPoint;
	float xPoint, yPoint, wH, hH;
	wH = m_width / 2.0f;
	hH = m_height / 2.0f;

	//As the screen coordinates go from -1 to 1, we need to determine where the x and y pixel is.
	if (_x < wH)
	{
		//results in a number between -1 and 0
		xPoint = -1 + ((float)_x / (wH));
	}
	else if (_x > wH)
	{
		//results in a number between 0 and 1
		xPoint = ((float)_x - wH) / wH;
	}
	else
	{
		xPoint = 0;
	}

	if (_y < hH)
	{
		yPoint = -1 + ((float)_y / (hH));
	}
	else if (_y > hH)
	{
		yPoint = ((float)_y - hH) / hH;
	}
	else
	{
		yPoint = 0;
	}

	///Now we know the coordinates of the ray origin.
	nearPoint.x = xPoint; nearPoint.y = yPoint; nearPoint.z = -1; nearPoint.w = 1;
	farPoint.x = xPoint; farPoint.y = yPoint; farPoint.z = 1; farPoint.w = 1;

	//now we need to convert this into eye space
	glm::vec4 eyeSpaceFar, eyeSpaceNear;
	eyeSpaceFar = m_invPM * farPoint;
	eyeSpaceFar = eyeSpaceFar / eyeSpaceFar.w;
	eyeSpaceNear = m_invPM * nearPoint;
	eyeSpaceNear = eyeSpaceNear / eyeSpaceNear.w;
	//now we need to convert this into world space
	glm::vec4 worldSpaceFar, worldSpaceNear;
	worldSpaceFar = m_invVM * eyeSpaceFar;
	worldSpaceNear = m_invVM * eyeSpaceNear;

	//we now know the ray origin and direction
	ret.origin = glm::vec3(worldSpaceNear);
	ret.direction = glm::normalize(glm::vec3(worldSpaceFar - worldSpaceNear));
	return ret;
}
