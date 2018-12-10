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


/*
TODO:

Have multiple iterations of the raytracer: no multithreading, multithreading, openMP; with low number of objects and high number of objects.
Video: explain the process of shading, shadows, reflections, transparancy and refraction. Show difference between frame rate fo

*/

void Bounce(int& bouncy, Tracer* rayTracer, float deltaT);


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
	Camera* cam = new Camera(WINDOWX, WINDOWY);
	Tracer* rayTracer = new Tracer();
	//                       Sphere(glm::vec3 _sphereCentre,float _radius, glm::vec3 _material,glm::vec3 _spec, float _reflectiveness, float _transparancy, float _refraction)
	rayTracer->AddObject(new Sphere(glm::vec3(0.65f,0.25f,0),       0.15, glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), 0.0f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.8f),     0.55, glm::vec3(0.2f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 0.7f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), -5.0f, 0.50f, 1.05f));
	rayTracer->AddObject(new Sphere(glm::vec3(1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.0015f, 0, 0));

	rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.15f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -1.00f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0, 0, 0));


	rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, 1, 1)));

	//tt->Init();
	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
	bool m_play = true;
	float currentT = 0;
	float lastT = 0;
	float deltaT = 0;
	Fps* fpsCounter = new Fps();
	int bouncy = 0;
	SDL_Event e;
	SDL_Rect name = { 0,0,255,35 };

	while (m_play)
	{
		//update time
		currentT = SDL_GetTicks();
		deltaT = currentT - lastT;
		lastT = currentT;
		fpsCounter->Update(currentT);
		
		//update SDL event
		while (SDL_PollEvent(&e) != 0) 
		{ 
			if( e.type == SDL_QUIT ) 
			{
				m_play = false; 
			}
		}
		Bounce(bouncy, rayTracer, deltaT);

		//draw the screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(m_renderer);
		///*
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
		//*/
		SpriteFactory::Draw("assets/single.png", name);
		fpsCounter->Draw();
		//SpriteFactory::Draw("assets/fps.png", name);
		SDL_RenderPresent(m_renderer);
	}

	//OPENMP
	m_play = true;
	while (m_play)
	{
		//update time
		currentT = SDL_GetTicks();
		deltaT = currentT - lastT;
		lastT = currentT;
		fpsCounter->Update(currentT);

		//update SDL event
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				m_play = false;
			}
		}
		Bounce(bouncy, rayTracer, deltaT);

		//draw the screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(m_renderer);

		#pragma omp parallel
		#pragma omp for 
//		#pragma omp collapse(2)
		for (int x = 0; x < WINDOWX; x++)
		{
			for (int y = 0; y < WINDOWY; y++)
			{
				Ray currentRay = cam->SpawnRay(x, y);
				glm::vec3 colour = rayTracer->RayTrace(&currentRay);

				#pragma omp critical 
				{ 
					SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
					SDL_RenderDrawPoint(m_renderer, x, y);
				}
			}
		}
		SpriteFactory::Draw("assets/open.png", name);
		fpsCounter->Draw();
		SDL_RenderPresent(m_renderer);
	}

	//MULTI-THREADING
	ThreadTrace* tt = new ThreadTrace(cam, rayTracer, m_renderer);
	m_play = true;
	while (m_play)
	{
		//update time
		currentT = SDL_GetTicks();
		deltaT = currentT - lastT;
		lastT = currentT;
		fpsCounter->Update(currentT);

		//update SDL event
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				m_play = false;
			}
		}
		Bounce(bouncy, rayTracer, deltaT);

		//draw the screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(m_renderer);
		tt->Perform();
		tt->Draw();
		SpriteFactory::Draw("assets/multi.png", name);
		fpsCounter->Draw();
		SDL_RenderPresent(m_renderer);
	}


	//////////////////////////
	/// FINISHED RAYTRACER ///
	//////////////////////////
	std::cout << "Fin ";
	//tt->DeInit();

	////////////////
	/// CLEAN UP ///
	////////////////
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	m_renderer = nullptr;
	m_window = nullptr;
	delete fpsCounter;
	delete tt;
	delete cam;
	delete rayTracer;

	return 0;
}

void Bounce(int& bouncy, Tracer* rayTracer, float deltaT)
{

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
		rayTracer->GetObjects()[0]->Translate(glm::vec3(-0.0001*deltaT, 0.0001*deltaT, 0));
		rayTracer->GetObjects()[1]->Translate(glm::vec3(0.0001*deltaT, -0.0001*deltaT, 0));
		rayTracer->GetObjects()[2]->Translate(glm::vec3(0, 0, 0.0001*deltaT));
		bouncy++;
		if (bouncy > 11)
		{
			bouncy = 0;
		}
	}
}