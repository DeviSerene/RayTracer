#include "SpriteFactory.h"

std::vector <std::shared_ptr<Sprite>> SpriteFactory::m_spriteList;
SDL_Renderer* SpriteFactory::m_renderer;
SDL_Window* SpriteFactory::m_window;

SpriteFactory::SpriteFactory()
{
}

SpriteFactory::~SpriteFactory()
{
	//m_spriteList.clear();
	if (!m_spriteList.empty())
	{
		for (int i = 0; i < m_spriteList.size(); i++) //delete all sprites in memory
		{
			//delete m_spriteList[i];
			m_spriteList[i] = nullptr;
		}
	}
	m_renderer = nullptr; //nullptr
}

std::shared_ptr<Sprite> SpriteFactory::GetSprite(std::string& _name, SDL_Rect& _position)
{
	


	//Could possibly be improved by a binary tree
	if (!m_spriteList.empty())
	{
		for (int i = 0; i < m_spriteList.size(); i++) //look through our list of sprites, if it is in memory, use it
		{
			if (_name == m_spriteList[i]->GetName()) //we have found the sprite
			{
				m_spriteList[i]->SetPos(ConvertRect(_position)); //update the position of where we want to draw this
				return m_spriteList[i]; //return it
			}
		}
	}

	//if it wasn't found then we need to add it
	CreateSprite(_name, ConvertRect(_position));
	//and return this new sprite 
	return m_spriteList[(m_spriteList.size()-1)];
}

void SpriteFactory::CreateSprite(std::string& _name, SDL_Rect& _position)
{
	//add a new sprite to memory
	m_spriteList.push_back(std::shared_ptr<Sprite>(new Sprite(m_renderer, _name, _position.x, _position.y, _position.w, _position.h)));
}

void SpriteFactory::DrawMouse(std::string _name, SDL_Rect _position)
{ 
	int width;
	int height;
	SDL_GetWindowSize(m_window, &width, &height);
	_position.w = ceil((float)_position.w * ((float)width / 1600.f));
	_position.h = ceil((float)_position.h * ((float)height / 900.f));

	if (!m_spriteList.empty())
	{
		for (int i = 0; i < m_spriteList.size(); i++) //look through our list of sprites, if it is in memory, use it
		{
			if (_name == m_spriteList[i]->GetName()) //we have found the sprite
			{
				m_spriteList[i]->SetPos(_position); //update the position of where we want to draw this
				m_spriteList[i]->Draw(); //return it
				return;
			}
		}
	}

	//if it wasn't found then we need to add it
	CreateSprite(_name, _position);
	//and return this new sprite 
	m_spriteList[(m_spriteList.size() - 1)]->Draw();

}

SDL_Rect SpriteFactory::ConvertRect(SDL_Rect _position)
{
	int width;
	int height;
	SDL_GetWindowSize(m_window, &width, &height);
	_position.x = (float)_position.x * ((float)width / 1600.f);
	_position.w = ceil((float)_position.w * ((float)width / 1600.f));
	_position.y = (float)_position.y * ((float)height / 900.f);
	_position.h = ceil((float)_position.h * ((float)height / 900.f));
	return _position;
}