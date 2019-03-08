#include "Button.h"

/// <summary>
/// Create a new button object.
/// Taking a Texture, text,
/// selected colour and 
/// default colour
/// </summary>
/// <param name="t">SDL_Texture *</param>
/// <param name="s">std::string</param>
/// <param name="c1">SDL_Color *</param>
/// <param name="c2">SDL_Color *</param>
Button::Button(SDL_Texture * t, std::string s, SDL_Color * c1, SDL_Color * c2):
	m_texture(t),
	m_text(s),
	m_selectedColour(c1),
	m_defaultColour(c2)
{
	m_rect = new SDL_Rect();
	m_rect->x = 100;
	m_rect->y = 100;
	m_rect->w = 100;
	m_rect->h = 50;

	const char * path = "assets/fonts/arial.ttf";
	m_font = TTF_OpenFont(path, 32);
}

/// <summary>
/// 
/// </summary>
Button::~Button()
{
}

/// <summary>
/// 
/// </summary>
void Button::update()
{

}

/// <summary>
/// 
/// </summary>
void Button::render(SDL_Renderer & renderer)
{
	SDL_RenderClear(&renderer);
	SDL_SetRenderDrawColor(&renderer, m_defaultColour->r, m_defaultColour->g,
		m_defaultColour->b, m_defaultColour->a);
	SDL_RenderDrawRect(&renderer, m_rect);

	m_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), *m_textColour);
	m_textTexture = SDL_CreateTextureFromSurface(&renderer, m_surface);

	SDL_RenderCopy(&renderer, m_textTexture, NULL, &m_textRect);

	SDL_RenderPresent(&renderer);

	SDL_FreeSurface(m_surface);
	SDL_DestroyTexture(m_textTexture);
}