#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "SpriteFactory.h"
#include <string>

///The number of frames to average by
#define FRAME_VALUES 10

///
///This class deals with finding and displaying the current FPS
/// This is very useful as we have different threading options.
///
class Fps
{
public:
	Fps();
	~Fps();

	///Update is used to find the fps
	void Update(float _currentTime);
	///Draw draws the sprites on screen to display the fps
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
	std::string m_writer;
	///The SDL_RECTS are for determining where to place the sprites on screen
	SDL_Rect m_fpsDisplay;
	SDL_Rect m_digitTens;
	SDL_Rect m_digitOnes;
	SDL_Rect m_digitPoint;
	SDL_Rect m_digitTenth;
	SDL_Rect m_digitHundreth;
	SDL_Rect m_digitThousanth;

};