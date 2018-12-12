#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "tracer.h"
#include "fps.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "threadtrace.h"
#include "SpriteFactory.h"
#include "GamestateManager.h"
#include "GameState.h"
#include "RayState.h"
#include <time.h>



int main()
{
	////////////////
	/// SDL INIT ///
	////////////////
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
		system("pause");
		return 0;
	}
	m_window = SDL_CreateWindow("Window", 125, 100, WINDOWX, WINDOWY, SDL_WINDOW_RESIZABLE);
	if (m_window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("pause");
		return 0;
	}
	m_renderer = SDL_CreateRenderer(m_window, -1,0); // SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	if (m_renderer == NULL)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		system("pause");
		return 0;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
		system("pause");
		return 0;
	}

	SpriteFactory::SetRenderer(m_renderer);
	SpriteFactory::SetWindow(m_window);

	//////////////////////
	/// INIT RAYTRACER ///
	//////////////////////
	srand(time(NULL));
	GamestateManager* gsm = new GamestateManager();
	gsm->AddState(new RayState(WINDOWX, WINDOWY));

	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
	bool m_play = true;
	
	while (m_play)
	{
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(m_renderer);
		m_play = gsm->HandleSDLEvents();
		gsm->Update();
		gsm->Draw();
		SDL_RenderPresent(m_renderer);
	}


	//////////////////////////
	/// FINISHED RAYTRACER ///
	//////////////////////////
	std::cout << "Fin ";
	gsm->RemoveLastState();
	delete gsm;

	////////////////
	/// CLEAN UP ///
	////////////////
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	m_renderer = nullptr;
	m_window = nullptr;

	return 0;
}
