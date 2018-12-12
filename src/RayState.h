#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "GameState.h"
#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "tracer.h"
#include "fps.h"
#include "threadtrace.h"

enum RAY_TRACE_TYPE
{
	RTT_Single, RTT_Para, RTT_Multi
};

/*!
This State deals with Raytracing in different ways;
*/
class RayState :
	public GameState
{
public:
	RayState(int windowX, int windowY);
	~RayState();
	bool HandleSDLEvents();						//handle inputs
	void Update();				//update 
	void Draw();								//draw to renderer
private:

	float RandomFloat(float min, float max);
	void Bounce();
	//The different rays for threading the raytracer
	void SingleThread();
	void MultiThread();
	void ParaThread();

	Camera* cam;
	Tracer* rayTracer;
	Fps* fpsCounter;

	float currentT = 0;
	float lastT = 0;
	float deltaT = 0;
	int bouncy = 0;
	SDL_Event e;
	SDL_Rect name;
	ThreadTrace* tt;

	RAY_TRACE_TYPE type;
};

