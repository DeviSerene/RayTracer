#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "SpriteFactory.h"
#include <string>

#define FRAME_VALUES 10

class Fps
{
public:
	Fps();
	~Fps();

	void Update(float _currentTime);
	void Draw();

private:

	// An array to store frame times:
	Uint32 m_frameTimes[FRAME_VALUES];
	// Last calculated SDL_GetTicks
	Uint32 m_frameTimeLast;
	// total frames rendered
	Uint32 m_frameCount;
	float m_framesPerSecond;
	Uint32 m_frameTimesIndex;
	Uint32 m_count;

	std::string m_fpsS;
	std::string writer;
	SDL_Rect m_fpsDisplay;
	SDL_Rect m_digitTens;
	SDL_Rect m_digitOnes;
	SDL_Rect m_digitPoint;
	SDL_Rect m_digitTenth;
	SDL_Rect m_digitHundreth;
	SDL_Rect m_digitThousanth;

};