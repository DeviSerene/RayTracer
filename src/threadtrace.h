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

class ThreadTrace
{
public:

	ThreadTrace(Camera* _cam, Tracer* _rayTracer, SDL_Renderer* _renderer)
	{
		m_cam = _cam; m_rayTracer = _rayTracer; m_renderer = _renderer;
		m_finished = false;
	}

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
				threadMu[m_currentT] = false;
				if (m_currentT < NUM_THREADS - 1)
					t[m_currentT] = std::thread([this] {this->Call(m_currentT, m_currentX, m_currentY, m_maxX, m_maxY); });
				++m_currentT; //(i*NUM_THREADS_Y) +i2
			}
		}
	}

	void DeInit()
	{
		m_finished = true;
		for (int ct = 0; ct < NUM_THREADS - 1; ct++)
		{
			//threadMu[ct].unlock();
			//	std::cout <<"Thread[" << ct << "] joined. \n";
			t[ct].join();
		}
	}

	void Loop()
	{
		for (int ct = 0; ct < NUM_THREADS; ct++)
		{
			if (ct < NUM_THREADS - 1)
			{
				threadMu[ct] = true;
			}
			else
				Call(m_currentX, m_currentY, m_maxX, m_maxY);
		}
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
					t[m_currentT] = std::thread([this] {this->Call(m_currentX, m_currentY, m_maxX, m_maxY); });
				else
					Call(m_currentX, m_currentY, m_maxX, m_maxY);
				++m_currentT; //(i*NUM_THREADS_Y) +i2
			}
		}


		//we should now join up all remaining threads to main
		for (int ct = 0; ct < NUM_THREADS - 1; ct++)
		{
			//	std::cout <<"Thread[" << ct << "] joined. \n";
			t[ct].join();
		}
	}

private:
	void Call(int _threadID, int _xStart, int _yStart, int _x, int _y)
	{
		while (!m_finished)
		{
			if (threadMu[_threadID])
			{
				for (int x = _xStart; x < _xStart + _x; x++)
				{
					for (int y = _yStart; y < _yStart + _y; y++)
					{//std::cout << "X: " << x << " Y: " << y << std::endl;
						Ray currentRay = m_cam->SpawnRay(x, y);
						glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

						//mu.lock();
						cols[((x*WINDOWY) + y)] = colour;
						//SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
						//SDL_RenderDrawPoint(m_renderer, x, y);
						//mu.unlock();
					}
				}
				threadMu[_threadID] = false;
			}
		}
		std::cout << "Thread finished.\n";
	}
	void Call(int _xStart, int _yStart, int _x, int _y)
	{
		for (int x = _xStart; x < _xStart + _x; x++)
		{
			for (int y = _yStart; y < _yStart + _y; y++)
			{//std::cout << "X: " << x << " Y: " << y << std::endl;
				Ray currentRay = m_cam->SpawnRay(x, y);
				glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

				//mu.lock();
				cols[((x*WINDOWY) + y)] = colour;
				//SDL_SetRenderDrawColor(m_renderer, colour.r, colour.g, colour.b, 255);
				//SDL_RenderDrawPoint(m_renderer, x, y);
				//mu.unlock();
			}
		}
		//std::cout << "Thread finished.\n";
	}

	int m_currentX;
	int m_currentY;
	int m_maxX;
	int m_maxY;
	int m_currentT;
	bool m_finished;
	glm::vec3 cols[WINDOWX*WINDOWY];
	std::thread t[NUM_THREADS];
	bool threadMu[NUM_THREADS];
	std::mutex mu;
	Camera* m_cam;
	Tracer* m_rayTracer;
	SDL_Renderer* m_renderer;
};