#include "RayState.h"

RayState::RayState(int windowX, int windowY)
	: GameState()
{

	cam = new Camera(windowX, windowY);
	rayTracer = new Tracer();
	fpsCounter = new Fps();
	rayTracer->AddObject(new Plane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1, 1, 1)));
	//                       Sphere(glm::vec3 _sphereCentre,float _radius, glm::vec3 _material,glm::vec3 _spec, float _reflectiveness, float _transparancy, float _refraction)
	rayTracer->AddObject(new Sphere(glm::vec3(0.65f, 0.25f, 0), 0.15, glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), 0.0f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-0.8f, 0, -0.8f), 0.55, glm::vec3(0.2f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 0.7f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(0.25f, -0.2f, -0.0f), 0.45, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), -5.0f, 0.50f, 1.05f));
	rayTracer->AddObject(new Sphere(glm::vec3(1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.0015f, 0, 0));

	rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -0.05f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0.15f, 0, 0));
	rayTracer->AddObject(new Sphere(glm::vec3(-1.0f, -1.00f, -2.0f), 0.45, glm::vec3(0.8f, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f), 0, 0, 0));



	tt = new ThreadTrace(cam, rayTracer, SpriteFactory::GetRenderer());
	name = { 0,0,255,35 };

	type = RTT_Single;
	//

}

RayState::~RayState()
{

	delete fpsCounter;
	delete tt;
	delete cam;
	delete rayTracer;
}

bool RayState::HandleSDLEvents()						//handle inputs
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym) //what key has been pressed?
			{
			case SDLK_ESCAPE:
				return false;
				break;
			case SDLK_RIGHT:
				switch (type)
				{
				case RTT_Single:
					type = RTT_Para;
					break;
				case RTT_Para:
					type = RTT_Multi;
					break;
				case RTT_Multi:
					type = RTT_Single;
					break;
				}
				break;
			case SDLK_LEFT:
				switch (type)
				{
				case RTT_Single:
					type = RTT_Multi;
					break;
				case RTT_Para:
					type = RTT_Single;
					break;
				case RTT_Multi:
					type = RTT_Para;
					break;
				}
				break;
			case SDLK_DOWN:
				//delete a sphere
				if (rayTracer->GetObjects().size() > 1)
				{
					rayTracer->RemoveObject();
				}
				break;
			case SDLK_UP:
				//add a sphere
				rayTracer->AddObject(new Sphere(
					glm::vec3(RandomFloat(-2.0f, 2.0f), RandomFloat(-2.0f, 0.0f), RandomFloat(-2.0f, 2.0f)),
					RandomFloat(0.1f, 0.9f),
					glm::vec3(RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f)),
					glm::vec3(RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f), RandomFloat(0.6f, 1.0f)),
					RandomFloat(-2.0f, 1.0f),
					RandomFloat(-2.0f, 1.0f),
					RandomFloat(1.0f, 1.1f)));

				break;
			case SDLK_w:
				cam->Move(glm::vec3(0.0f, 0.0f, 0.1f));
				break;
			case SDLK_s:
				cam->Move(glm::vec3(0.0f, 0.0f, -0.1f));
				break;
			case SDLK_d:
				cam->Move(glm::vec3(-0.1f, 0.0f, 0.0f));
				break;
			case SDLK_a:
				cam->Move(glm::vec3(0.1f, 0.0f, 0.0f));
				break;
			}
		}
	}
	return true;
}

void RayState::Update()				//update 
{
	//update time
	currentT = SDL_GetTicks();
	deltaT = currentT - lastT;
	lastT = currentT;
	fpsCounter->Update(currentT);

	Bounce();

	if (type == RTT_Multi)
	{
		tt->Perform();
	}
}

void RayState::Draw()								//draw to renderer
{

	switch (type)
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
	fpsCounter->Draw();
}

void RayState::Bounce()
{
	float amount = deltaT * 0.0001;
	//Bouncing balls
	if (rayTracer->GetObjects().size() >= 4)
	{
		if (bouncy <= 5)
		{
			rayTracer->GetObjects()[1]->Translate(glm::vec3(amount, -amount, 0));
			rayTracer->GetObjects()[2]->Translate(glm::vec3(-amount, amount, 0));
			rayTracer->GetObjects()[3]->Translate(glm::vec3(0, 0, -amount));
			bouncy++;
		}
		else
		{
			rayTracer->GetObjects()[1]->Translate(glm::vec3(-amount, amount, 0));
			rayTracer->GetObjects()[2]->Translate(glm::vec3(amount, -amount, 0));
			rayTracer->GetObjects()[3]->Translate(glm::vec3(0, 0, amount));
			bouncy++;
			if (bouncy > 11)
			{
				bouncy = 0;
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
			Ray currentRay = cam->SpawnRay(x, y);
			glm::vec3 colour = rayTracer->RayTrace(&currentRay);

			SDL_SetRenderDrawColor(SpriteFactory::GetRenderer(), colour.r, colour.g, colour.b, 255);
			SDL_RenderDrawPoint(SpriteFactory::GetRenderer(), x, y);
		}
	}
	SpriteFactory::Draw("assets/single.png", name);
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
			Ray currentRay = cam->SpawnRay(x, y);
			glm::vec3 colour = rayTracer->RayTrace(&currentRay);

			#pragma omp critical 
			{
				SDL_SetRenderDrawColor(SpriteFactory::GetRenderer(), colour.r, colour.g, colour.b, 255);
				SDL_RenderDrawPoint(SpriteFactory::GetRenderer(), x, y);
			}
		}
	}
	SpriteFactory::Draw("assets/open.png", name);
}

void RayState::MultiThread()
{
	tt->Draw();
	SpriteFactory::Draw("assets/multi.png", name);
	//
	for (int i = 0; i < tt->GetThreadCount(); i++)
	{
		std::string file = "assets/" + std::to_string(i) + ".png";
		SpriteFactory::Draw("assets/thread.png", { 0,60 + (16 * i),100,15 });
		SpriteFactory::Draw(file, { 110, 60 + (16 * i),15,15 });
		std::string number = std::to_string(tt->GetTime(i)/1000.0f);
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