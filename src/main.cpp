#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "tracer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define WINDOWX 640
#define WINDOWY 480
#define NUM_THREADS 4

class ThreadTrace
{
public:

	ThreadTrace(Camera* _cam, Tracer* _rayTracer, SDL_Renderer* _renderer) 
	{
		m_cam = _cam; m_rayTracer = _rayTracer; m_renderer = _renderer;
	}

	void Perform()
	{
		m_currentX = 0; m_currentY = 0; m_currentT = 0;

		for (int x = 0; x < WINDOWX; x++)
		{
			m_currentX = x;
			for (int y = 0; y < WINDOWY; y++)
			{
				m_currentY = y;
				//Launch a new thread, creating a ray at the xy location
				//std::cout << "Thread[" << m_currentT << "] launched. \n";
				t[m_currentT] = std::thread([this] {this->Call(m_currentX, m_currentY); });

				//if we are currently using all the threads, wait until they are finished
				m_currentT++;
				if (m_currentT >= NUM_THREADS)
				{
					m_currentT = 0;
					for (int ct = 0; ct < NUM_THREADS; ct++)
					{
				//		std::cout << "Thread[" << ct << "] joined. \n";
						t[ct].join();
					}
				}
			}
		}

		//we should now join up all remaining threads to main
		for (int ct = 0; ct < m_currentT; ct++)
		{
		//	std::cout <<"Thread[" << ct << "] joined. \n";
			t[ct].join();
		}
	}

private:
	void Call(int x, int y)
	{
		//std::cout << "X: " << x << " Y: " << y << std::endl;
		Ray currentRay = m_cam->SpawnRay(x, y);
		glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

		mu.lock();
		//cols[x*y] = colour;
		SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
		SDL_RenderDrawPoint(m_renderer, x, y);
		mu.unlock();
	}

	int m_currentX;
	int m_currentY;
	int m_currentT;
	std::thread t[NUM_THREADS];
	std::mutex mu;
	Camera* m_cam;
	Tracer* m_rayTracer;
	SDL_Renderer* m_renderer;
};

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
	//                       Sphere(glm::vec3 _sphereCentre,float _radius, glm::vec3 _material,glm::vec3 _spec, float _reflectiveness, float _transparancy, float _refraction)
	rayTracer->AddObject(new Sphere(glm::vec3(0.65f,0.25f,0),       0.15, glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), 0.1f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.8f),     0.55, glm::vec3(0.2f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 0.7f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), -5.0f, 0.75f, 1.3f));
	rayTracer->AddObject(new Sphere(glm::vec3(1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.5f, 0, 0));
	rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, 1, 1)));

	ThreadTrace* tt = new ThreadTrace(cam, rayTracer, m_renderer);
	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
	bool m_play = true;
	float currentT = 0;
	float lastT = 0;
	float deltaT = 0;
	int counter = 0;
	float fps = 0;
	int bouncy = 0;
	SDL_Event e;
	while (m_play)
	{
		counter++;
		//update time
		currentT = SDL_GetTicks(); deltaT = currentT - lastT;
		lastT = currentT;
		fps += deltaT;
		if (fps >= 1)
		{
			std::cout << "FPS: " << counter << std::endl;
			counter = 0;
			fps = 0;
		}
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
		//std::cout << "START ";
		//tt->Perform();
		//std::cout << "DONE ";
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