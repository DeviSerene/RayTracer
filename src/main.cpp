#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "tracer.h"
#include <SDL2/SDL.h>
#include <iostream>

#define WINDOWX 640
#define WINDOWY 480

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

	//////////////////////
	/// INIT RAYTRACER ///
	//////////////////////
	Camera* cam = new Camera(WINDOWX, WINDOWY);
	Tracer* rayTracer = new Tracer();
	rayTracer->AddObject(new Sphere(glm::vec3(0.65f,0.25f,0), 0.15, glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -3.8f), 0.55, glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)));
	rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)));
	rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)));

	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
	bool m_play = true;
	float currentT = 0;
	float lastT = 0;
	float deltaT = 0;
	int bouncy = 0;
	SDL_Event e;
	while (m_play)
	{
		//update time
		currentT = SDL_GetTicks(); deltaT = currentT - lastT;
		lastT = currentT;
		//update SDL event
		while (SDL_PollEvent(&e) != 0) 
		{ 
			if( e.type == SDL_QUIT ) 
			{
				m_play = false; 
			}
		}
		//Bouncing balls
		if (bouncy <= 5)
		{
			rayTracer->GetObjects()[0]->Translate(glm::vec3(0.0001*deltaT, -0.0001*deltaT, 0));
			rayTracer->GetObjects()[1]->Translate(glm::vec3(-0.0001*deltaT, 0.0001*deltaT, 0));
			rayTracer->GetObjects()[2]->Translate(glm::vec3(0, 0, -0.0001*deltaT));
			bouncy++;
		}
		else
		{
			rayTracer->GetObjects()[0]->Translate(glm::vec3(-0.0001*deltaT, 0.0001*deltaT,0));
			rayTracer->GetObjects()[1]->Translate(glm::vec3(0.0001*deltaT, -0.0001*deltaT, 0));
			rayTracer->GetObjects()[2]->Translate(glm::vec3(0, 0, 0.0001*deltaT));
			bouncy++;
			if (bouncy > 11)
			{
				bouncy = 0;
			}
		}

		//draw the screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(m_renderer);
		for (int x = 0; x < WINDOWX; x++)
		{
			for (int y = 0; y < WINDOWY; y++)
			{
				Ray currentRay = cam->SpawnRay(x, y);
				glm::vec3 colour = rayTracer->RayTrace(&currentRay);

				SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
				SDL_RenderDrawPoint(m_renderer, x, y);
			}
		}
		SDL_RenderPresent(m_renderer);
	}

	//////////////////////////
	/// FINISHED RAYTRACER ///
	//////////////////////////
	std::cout << "Fin ";

	////////////////
	/// CLEAN UP ///
	////////////////
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	m_renderer = nullptr;
	m_window = nullptr;
	delete cam;
	delete rayTracer;

	return 0;
}