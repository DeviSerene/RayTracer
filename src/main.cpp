#include "camera.h"
#include "object.h"
#include "sphere.h"
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
	SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(m_renderer);

	//////////////////////
	/// INIT RAYTRACER ///
	//////////////////////
	Camera* cam = new Camera(WINDOWX, WINDOWY);
	Tracer* rayTracer = new Tracer();
	rayTracer->AddObject(new Sphere(glm::vec3(0.1f,-0.15f,0), 0.15, glm::vec3(0, 1, 0), glm::vec3(1, 1, 1)));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.1f), 0.65, glm::vec3(0, 1, 1), glm::vec3(1, 1, 1)));
	rayTracer->AddObject(new Sphere(glm::vec3(0.05f, -0.2f, -1.0f), 0.45, glm::vec3(1, 1, 0), glm::vec3(1, 1, 1)));

	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
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

	//////////////////////////
	/// FINISHED RAYTRACER ///
	//////////////////////////
	SDL_RenderPresent(m_renderer);
	std::cout << "Fin ";
	std::system("pause");

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