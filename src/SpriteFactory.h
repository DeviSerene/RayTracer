#pragma once
#include <SDL2/SDL.h>
#include "Sprite.h"
#include <vector>
#include <string>

class SpriteFactory //an organisational class that allows me to draw multiples of the same sprite, saving memory
{
public:
	static void SetRenderer(SDL_Renderer* _renderer) {m_renderer = _renderer;} //the sprite factory now has a reference to the renderer, so it can create sprites
	static void SetWindow(SDL_Window* _window) { m_window = _window; }
	
	static void Draw(std::string _name, SDL_Rect _position) {GetSprite(_name, _position)->Draw(); }
	static SDL_Renderer* GetRenderer() { return m_renderer; }
	static SDL_Rect ConvertRect(SDL_Rect _position);
private:
	SpriteFactory();
	~SpriteFactory();

	static std::shared_ptr<Sprite> GetSprite(std::string& _name, SDL_Rect& _position); //return a sprite from the spritelist, or call Create Sprite
	static void CreateSprite(std::string& _name, SDL_Rect& _position); //adds a sprite to spritelist

	static SDL_Renderer* m_renderer; //a reference to the renderer, for the creation of sprites
	static SDL_Window* m_window; //a reference to the window, for resizing sprites
	static std::vector<std::shared_ptr<Sprite>> m_spriteList; //a list of all sprites in memory.
};

