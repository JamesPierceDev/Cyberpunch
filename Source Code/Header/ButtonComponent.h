#pragma once
#include "Component.h"
#include "SDL.h"

class ButtonComponent : public Component
{
public:
	ButtonComponent(SDL_Texture* selectedTexture, SDL_Texture* unselectedTexture, std::string tag, bool selected = false) :
		m_tag(tag),
		m_selected(selected),
		m_selectedTexture(selectedTexture),
		m_deselectTexture(unselectedTexture)
	{
		//Set the current texture
		m_currentTexture = m_selected ? m_selectedTexture : m_deselectTexture;
	}
	void select()
	{
		m_selected = true;
		m_currentTexture = m_selectedTexture;
	}
	void deselect()
	{
		m_selected = false;
		m_currentTexture = m_deselectTexture;
	}

	SDL_Texture* getTexture() { return m_currentTexture; }
	std::string& getTag() { return m_tag; }
private:
	std::string m_tag;
	bool m_selected;
	SDL_Texture * m_selectedTexture, *m_deselectTexture, *m_currentTexture;
};