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
This State deals with Raytracing in different ways: Single Threading, Paralelle Threading and MultiThreading
*/
class RayState :
	public GameState
{
public:
	RayState(int windowX, int windowY);
	~RayState();
	bool HandleSDLEvents();		///handle inputs
	void Update();				///update 
	void Draw();				///draw to renderer
private:

	///Randomise a float from min to max
	float RandomFloat(float min, float max);
	///Make the 3 balls bounce up and down
	void Bounce();
	int m_bounceCounter = 0;
	///The different rays for threading the raytracer
	void SingleThread();
	void MultiThread();
	void ParaThread();

	///The camera is our lens into the 3d world, and creator of the initial rays
	Camera* m_cam;
	///The tracer traces the rays through the world, determining what colour to draw
	Tracer* m_rayTracer;
	///the counter to display and find out the current fps
	Fps* m_fpsCounter;

	///the time
	float m_currentT = 0;
	float m_lastT = 0;
	float m_deltaT = 0;
	///SDL event
	SDL_Event m_ev;
	///The position for the name display
	SDL_Rect m_displayRect;
	///This class deals with multi-threading the raytracing
	ThreadTrace* m_multiThreading;
	///What threading type are we using?
	RAY_TRACE_TYPE m_traceType;
};

