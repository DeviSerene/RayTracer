#include "RayState.h"

RayState::RayState(int windowX, int windowY)
	: GameState()
{

	m_cam = new Camera(windowX, windowY);
	m_rayTracer = new Tracer();
	m_fpsCounter = new Fps();
	m_rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, 1, 1)));
	//                       Sphere(glm::vec3 _sphereCentre,float _radius, glm::vec3 _material,glm::vec3 _spec, float _reflectiveness, float _transparancy, float _refraction)
	m_rayTracer->AddObject(new Sphere(glm::vec3(0.65f, 0.25f, 0), 0.15, glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), 0.0f, 0, 0));
	m_rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.8f), 0.55, glm::vec3(0.2f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 0.7f, 0, 0));
	m_rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), -5.0f, 0.50f, 1.05f));
	m_rayTracer->AddObject(new Sphere(glm::vec3(1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.0015f, 0, 0));

	m_rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.15f, 0, 0));
	m_rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -1.00f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0, 0, 0));



	m_multiThreading = new ThreadTrace(m_cam, m_rayTracer, SpriteFactory::GetRenderer());
	m_displayRect = { 0,0,255,35 };

	m_traceType = RTT_Single;
	//

}

RayState::~RayState()
{

	delete m_fpsCounter;
	delete m_multiThreading;
	delete m_cam;
	delete m_rayTracer;
}

bool RayState::HandleSDLEvents()						//handle inputs
{
	while (SDL_PollEvent(&m_ev) != 0)
	{
		if (m_ev.type == SDL_QUIT)
		{
			return false;
		}
		if (m_ev.type == SDL_KEYDOWN)
		{
			switch (m_ev.key.keysym.sym) //what key has been pressed?
			{
			case SDLK_ESCAPE:
				return false;
				break;
			case SDLK_RIGHT:
				switch (m_traceType)
				{
				case RTT_Single:
					m_traceType = RTT_Para;
					break;
				case RTT_Para:
					m_traceType = RTT_Multi;
					break;
				case RTT_Multi:
					m_traceType = RTT_Single;
					break;
				}
				break;
			case SDLK_LEFT:
				switch (m_traceType)
				{
				case RTT_Single:
					m_traceType = RTT_Multi;
					break;
				case RTT_Para:
					m_traceType = RTT_Single;
					break;
				case RTT_Multi:
					m_traceType = RTT_Para;
					break;
				}
				break;
			case SDLK_DOWN:
				//delete a sphere
				if (m_rayTracer->GetObjects().size() > 1)
				{
					m_rayTracer->RemoveObject();
				}
				break;
			case SDLK_UP:
				//add a sphere
				m_rayTracer->AddObject(new Sphere(
					glm::vec3(RandomFloat(-2.0f, 2.0f), RandomFloat(-2.0f, 0.0f), RandomFloat(-2.0f, 2.0f)),
					RandomFloat(0.1f, 0.9f),
					glm::vec3(RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f)),
					glm::vec3(RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f)),
					RandomFloat(-2.0f, 1.0f),
					RandomFloat(-2.0f, 1.0f),
					RandomFloat(1.0f, 1.1f)));

				break;
			case SDLK_w:
				m_cam->Move(glm::vec3(0.0f, 0.0f, 0.1f));
				break;
			case SDLK_s:
				m_cam->Move(glm::vec3(0.0f, 0.0f, -0.1f));
				break;
			case SDLK_d:
				m_cam->Move(glm::vec3(-0.1f, 0.0f, 0.0f));
				break;
			case SDLK_a:
				m_cam->Move(glm::vec3(0.1f, 0.0f, 0.0f));
				break;
			}
		}
	}
	return true;
}

void RayState::Update()				//update 
{
	//update time
	m_currentT = SDL_GetTicks();
	m_deltaT = m_currentT - m_lastT;
	m_lastT = m_currentT;
	m_fpsCounter->Update(m_currentT);

	Bounce();

	if (m_traceType == RTT_Multi)
	{
		m_multiThreading->Perform();
	}
}

void RayState::Draw()								//draw to renderer
{

	switch (m_traceType)
	{
	case RTT_Single:
		SingleThread();
		break;
	case RTT_Para:
		ParaThread();
		break;
	case RTT_Multi:
		MultiThread();
		break;

	}
	m_fpsCounter->Draw();
}

void RayState::Bounce()
{
	float amount = m_deltaT * 0.0001;
	//Bouncing balls
	if (m_rayTracer->GetObjects().size() >= 4)
	{
		if (m_bounceCounter <= 5)
		{
			m_rayTracer->GetObjects()[1]->Translate(glm::vec3(amount, -amount, 0));
			m_rayTracer->GetObjects()[2]->Translate(glm::vec3(-amount, amount, 0));
			m_rayTracer->GetObjects()[3]->Translate(glm::vec3(0, 0, -amount));
			m_bounceCounter++;
		}
		else
		{
			m_rayTracer->GetObjects()[1]->Translate(glm::vec3(-amount, amount, 0));
			m_rayTracer->GetObjects()[2]->Translate(glm::vec3(amount, -amount, 0));
			m_rayTracer->GetObjects()[3]->Translate(glm::vec3(0, 0, amount));
			m_bounceCounter++;
			if (m_bounceCounter > 11)
			{
				m_bounceCounter = 0;
			}
		}
	}
}

void RayState::SingleThread()
{
	for (int x = 0; x < WINDOWX; x++)
	{
		for (int y = 0; y < WINDOWY; y++)
		{
			Ray currentRay = m_cam->SpawnRay(x, y);
			glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

			SDL_SetRenderDrawColor(SpriteFactory::GetRenderer(), colour.r, colour.g, colour.b, 255);
			SDL_RenderDrawPoint(SpriteFactory::GetRenderer(), x, y);
		}
	}
	SpriteFactory::Draw("assets/single.png", m_displayRect);
}

void RayState::ParaThread()
{
#pragma omp parallel
#pragma omp for 
	//		#pragma omp collapse(2)
	for (int x = 0; x < WINDOWX; x++)
	{
		for (int y = 0; y < WINDOWY; y++)
		{
			Ray currentRay = m_cam->SpawnRay(x, y);
			glm::vec3 colour = m_rayTracer->RayTrace(&currentRay);

			#pragma omp critical 
			{
				SDL_SetRenderDrawColor(SpriteFactory::GetRenderer(), colour.r, colour.g, colour.b, 255);
				SDL_RenderDrawPoint(SpriteFactory::GetRenderer(), x, y);
			}
		}
	}
	SpriteFactory::Draw("assets/open.png", m_displayRect);
}

void RayState::MultiThread()
{
	m_multiThreading->Draw();
	SpriteFactory::Draw("assets/multi.png", m_displayRect);
	//
	for (int i = 0; i < m_multiThreading->GetThreadCount(); i++)
	{
		std::string file = "assets/" + std::to_string(i) + ".png";
		SpriteFactory::Draw("assets/thread.png", { 0,60 + (16 * i),100,15 });
		SpriteFactory::Draw(file, { 110, 60 + (16 * i),15,15 });
		std::string number = std::to_string(m_multiThreading->GetTime(i)/1000.0f);
		for (int f = 0; f < 5; f++)
		{
			file = "assets/";
			file += number.at(f);
			file += ".png";
			SpriteFactory::Draw(file, { 150 + (16 * f),60 + (16 * i),15,15 });
		}
	}
}

float RayState::RandomFloat(float min, float max)
{
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}