#pragma once
#include "camera.h"
#include "tracer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define NUM_THREADS 4
#define WINDOWX 640
#define WINDOWY 480

///This class deals with multithreading the raytracer
class ThreadTrace
{
public:

	ThreadTrace(Camera* _cam, Tracer* _rayTracer, SDL_Renderer* _renderer)
	{
		m_cam = _cam; m_rayTracer = _rayTracer; m_renderer = _renderer;
		m_finished = false;
		//std::thread::hardware_concurrency();
	}

	///This will create active threads, ready to be used
	void Init()
	{
		m_currentX = 0; m_currentY = 0; m_currentT = 0;

		m_maxX = (WINDOWX / (NUM_THREADS / 2));
		m_maxY = (WINDOWY / (NUM_THREADS / 2));
		for (int i = 0; i < (NUM_THREADS / 2); i++)
		{
			m_currentX = i * m_maxX;
			for (int i2 = 0; i2 < (NUM_THREADS / 2); i2++)
			{
				m_currentY = i2 * m_maxY;
				//		std::cout <<"Thread[" << m_currentT << "] started. \n";
				m_activateThread[m_currentT] = false;
				if (m_currentT < NUM_THREADS - 1)
					m_threads[m_currentT] = std::thread([this] {this->CreateThread(m_currentT, m_currentX, m_currentY, m_maxX, m_maxY); });
				++m_currentT; //(i*NUM_THREADS_Y) +i2
			}
		}
	}

	///this will close all threads
	void DeInit()
	{
		m_finished = true;
		for (int ct = 0; ct < NUM_THREADS - 1; ct++)
		{
			//threadMu[ct].unlock();
			//	std::cout <<"Thread[" << ct << "] joined. \n";
			m_threads[ct].join();
		}
	}

	///This loops thru the created threads to perform the raytracer
	void Loop()
	{
		for (int ct = 0; ct < NUM_THREADS; ct++)
		{
			if (ct < NUM_THREADS - 1)
			{
				m_activateThread[ct] = true;
			}
			else
				Call(m_currentX, m_currentY, m_maxX, m_maxY,0);
		}
	}

	///this draws the collected colours on screen;
	void Draw()
	{
		for (int x = 0; x < WINDOWX; x++)
		{
			for (int y = 0; y < WINDOWY; y++)
			{
				glm::vec3 color = m_colours[((x*WINDOWY) + y)];
				SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
				SDL_RenderDrawPoint(m_renderer, x, y);
			}
		}
	}

	///This creates the threads to perform the raytracing; this is a different method to the one above.
	void Perform()
	{
		m_currentX = 0; m_currentY = 0; m_currentT = 0;

		m_maxX = (WINDOWX / (NUM_THREADS / 2));
		m_maxY = (WINDOWY / (NUM_THREADS / 2));
		for (int i = 0; i < (NUM_THREADS / 2); i++)
		{
			m_currentX = i * m_maxX;
			for (int i2 = 0; i2 < (NUM_THREADS / 2); i2++)
			{
				m_currentY = i2 * m_maxY;
				//		std::cout <<"Thread[" << m_currentT << "] started. \n";
				if (m_currentT < NUM_THREADS - 1)
					m_threads[m_currentT] = std::thread([this] {this->Call(m_currentX, m_currentY, m_maxX, m_maxY, m_currentT); }); //create a thread
				else
					Call(m_currentX, m_currentY, m_maxX, m_maxY, m_currentT); //perform on the main thread
				++m_currentT; //(i*NUM_THREADS_Y) +i2
			}
		}


		//we should now join up all remaining threads to main
		for (int ct = 0; ct < NUM_THREADS - 1; ct++)
		{
			//	std::cout <<"Thread[" << ct << "] joined. \n";
			m_threads[ct].join();
		}
	}

	float GetTime(int m_threads) { return m_time[m_threads]; }
	int GetThreadCount() { return NUM_THREADS; }

private:
	void CreateThread(int _threadID, int _xStart, int _yStart, int _x, int _y)
	{
		while (!m_finished)
		{
			if (m_activateThread[_threadID])
			{
				for (int x = _xStart; x < _xStart + _x; x++)
				{
					for (int y = _yStart; y < _yStart + _y; y++)
					{//std::cout << "X: " << x << " Y: " << y << std::endl;
						Ray currentRay = m_cam->SpawnRay(x, y);
						glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

						//mu.lock();
						m_colours[((x*WINDOWY) + y)] = colour;
						//SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
						//SDL_RenderDrawPoint(m_renderer, x, y);
						//mu.unlock();
					}
				}
				m_activateThread[_threadID] = false;
			}
		}
		std::cout << "Thread finished.\n";
	}


	void Call(int _xStart, int _yStart, int _x, int _y, int _threadID)
	{
		float start = SDL_GetTicks();
		for (int x = _xStart; x < _xStart + _x; x++)
		{
			for (int y = _yStart; y < _yStart + _y; y++)
			{//std::cout << "X: " << x << " Y: " << y << std::endl;
				Ray currentRay = m_cam->SpawnRay(x, y);
				glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

				//mu.lock();
				m_colours[((x*WINDOWY) + y)] = colour;
				//SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
				//SDL_RenderDrawPoint(m_renderer, x, y);
				//mu.unlock();
			}
		}
		m_time[_threadID] = SDL_GetTicks() - start;
		//std::cout << "Thread finished.\n";
	}



	int m_currentX;
	int m_currentY;
	int m_maxX;
	int m_maxY;
	int m_currentT;
	bool m_finished;
	float m_time[NUM_THREADS];
	glm::vec3 m_colours[WINDOWX*WINDOWY];
	std::thread m_threads[NUM_THREADS];
	bool m_activateThread[NUM_THREADS];
	std::mutex m_mutex;
	Camera* m_cam;
	Tracer* m_rayTracer;
	SDL_Renderer* m_renderer;
};