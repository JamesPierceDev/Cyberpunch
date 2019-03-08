#pragma once
#include "Component.h"
#include "SDL.h"

class PlatformComponent : public Component
{
public:
	PlatformComponent() {}
	~PlatformComponent()
	{
		//Free the memory our pointers took up
		delete m_blue;
		delete m_green;
		delete m_pink;
		delete m_orange;
	}

	void setBlue(SDL_Texture* blue) { m_blue = blue; }
	void setGreen(SDL_Texture* green) { m_green = green; }
	void setPink(SDL_Texture* pink) { m_pink = pink; }
	void setOrange(SDL_Texture* orange) { m_orange = orange; }

	SDL_Texture* getTexture(std::string bg)
	{
		if (bg == "Game BG0")
			return m_green;
		else if (bg == "Game BG1")
			return m_orange;
		else if (bg == "Game BG2")
			return m_blue;
		else
			return m_pink;
	}
private:
	//All the different colours for the platforms
	SDL_Texture * m_blue, *m_green, *m_pink, *m_orange;
};