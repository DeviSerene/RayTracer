#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>

class Sprite
{
public:
	Sprite(SDL_Renderer* _renderer, std::string _file, int _x, int _y, int _w, int _h);
	~Sprite();

	void Draw();
	void DrawTint(Uint8 _r, Uint8 _g, Uint8 _b);
	void AniDraw(SDL_Rect& _cellRect); //draw for animations
	void TintAniDraw(SDL_Rect& _cellRect, Uint8 _r, Uint8 _g, Uint8 _b, bool flip); //draw for animations

	std::string& GetName() { return m_name; } //used by SpriteFactory to locate sprites
	void SetPos(SDL_Rect _position) { m_position = _position; } //used by SpriteFactory to update the position

protected:
	SDL_Renderer* m_renderer; //sdl renderer used for drawing
	SDL_Texture* m_image; //a pointer to the image in memory
	SDL_Rect m_position; //where to draw this image

	std::string m_name;
};

