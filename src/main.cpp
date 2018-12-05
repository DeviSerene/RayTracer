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
#define NUM_THREADS_X 4
#define NUM_THREADS_Y 4
#define FRAME_VALUES 10

class ThreadTrace
{
public:

	ThreadTrace(Camera* _cam, Tracer* _rayTracer, SDL_Renderer* _renderer) 
	{
		m_cam = _cam; m_rayTracer = _rayTracer; m_renderer = _renderer;
	}

	void Draw()
	{
		for (int x = 0; x < WINDOWX; x++)
		{
			for (int y = 0; y < WINDOWY; y++)
			{
				glm::vec3 color = cols[((x*WINDOWY) + y)];
				SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
				SDL_RenderDrawPoint(m_renderer, x, y);
			}
		}
	}

	void Perform()
	{
		m_currentX = 0; m_currentY = 0; m_currentT = 0;

		m_maxX = (WINDOWX / NUM_THREADS_X);
		m_maxY = (WINDOWY / NUM_THREADS_Y);
		for (int i = 0; i < NUM_THREADS_X; i++)
		{
			m_currentX = i * m_maxX;
			for (int i2 = 0; i2 < NUM_THREADS_Y; i2++)
			{
				m_currentY = i2 * m_maxY;
		//		std::cout <<"Thread[" << m_currentT << "] started. \n";
				t[m_currentT] = std::thread([this] {this->Call(m_currentX, m_currentY, m_maxX, m_maxY); });
				++m_currentT; //(i*NUM_THREADS_Y) +i2
			}
		}

		
		//we should now join up all remaining threads to main
		for (int ct = 0; ct < NUM_THREADS_X*NUM_THREADS_Y; ct++)
		{
		//	std::cout <<"Thread[" << ct << "] joined. \n";
			t[ct].join();
		}
		//*/
	}

private:
	void Call(int _xStart, int _yStart, int _x, int _y)
	{
		for (int x = _xStart; x < _xStart+_x; x++)
		{
			for (int y = _yStart; y < _yStart+ _y; y++)
			{//std::cout << "X: " << x << " Y: " << y << std::endl;
				Ray currentRay = m_cam->SpawnRay(x, y);
				glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

				mu.lock();
				cols[((x*WINDOWY) + y)] = colour;
				//SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
				//SDL_RenderDrawPoint(m_renderer, x, y);
				mu.unlock();
			}
		}
		//std::cout << "Thread finished.\n";
	}

	int m_currentX;
	int m_currentY;
	int m_maxX;
	int m_maxY;
	int m_currentT;
	glm::vec3 cols[WINDOWX*WINDOWY];
	std::thread t[NUM_THREADS_X*NUM_THREADS_Y];
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
	rayTracer->AddObject(new Sphere(glm::vec3(0.65f,0.25f,0),       0.15, glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), 0.0f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.8f),     0.55, glm::vec3(0.2f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 0.7f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), -5.0f, 0.75f, 1.15f));
	rayTracer->AddObject(new Sphere(glm::vec3(1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.0015f, 0, 0));
	rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, 1, 1)));

	ThreadTrace* tt = new ThreadTrace(cam, rayTracer, m_renderer);
	/////////////////////////
	/// PERFORM RAYTRACER ///
	/////////////////////////
	bool m_play = true;
	float currentT = 0;
	float lastT = 0;
	float deltaT = 0;

	// An array to store frame times:
	Uint32 frametimes[FRAME_VALUES];
	memset(frametimes, 0, sizeof(frametimes));
	// Last calculated SDL_GetTicks
	Uint32 frametimelast = SDL_GetTicks();
	// total frames rendered
	Uint32 framecount = 0;
	float framespersecond = 0;
	Uint32 frametimesindex;
	Uint32 count;
	// the value you want
	
	int bouncy = 0;
	SDL_Event e;
	while (m_play)
	{
		//update time
		currentT = SDL_GetTicks();
		deltaT = currentT - lastT;
		lastT = currentT;

		//FPS
		// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
		// This value rotates back to 0 after it hits FRAME_VALUES.
		frametimesindex = framecount % FRAME_VALUES;
		// store the current time
		// save the frame time value
		frametimes[frametimesindex] = currentT - frametimelast;
		// save the last frame time for the next fpsthink
		frametimelast = currentT;
		// increment the frame count
		framecount++;
		if (framecount < FRAME_VALUES) {count = framecount;}
		else {count = FRAME_VALUES;}
		// add up all the values and divide to get the average frame time.
		framespersecond = 0;
		for (int i = 0; i < count; i++) {framespersecond += frametimes[i];}
		framespersecond /= count;
		// now to make it an actual frames per second value...
		framespersecond = 1000.f / framespersecond;

		std::cout << "FPS: " << framespersecond << std::endl;

		
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
		tt->Perform();
		tt->Draw();
		/*
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