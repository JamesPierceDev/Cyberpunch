#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "../Libraries/SDL_TTF/include/SDL_ttf.h"
#include "../Libraries/SDL_image/include/SDL_image.h"
#include <string>
#include <iostream>

class Button
{
public:
	Button(SDL_Texture*, std::string, SDL_Color *, SDL_Color *);
	~Button();

	void update();
	void render(SDL_Renderer &);
private:
	SDL_Rect * m_rect;
	SDL_Surface * m_surface;
	SDL_Texture * m_texture;
	SDL_Color * m_selectedColour;
	SDL_Color * m_defaultColour;

	TTF_Font * m_font;
	SDL_Rect m_textRect;
	SDL_Color  * m_textColour;
	SDL_Texture * m_textTexture;

	std::string m_text;
	bool selected = false;
};

#endif