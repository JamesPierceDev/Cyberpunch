#pragma once
#include "Scene.h"
#include "Camera.h"
#include "Entity.h"
#include "OnlineSystem.h"
#include "ButtonComponent.h"

class LobbyScene : public Scene {
public:
	LobbyScene();
	void start();
	void stop();
	void update(double dt);
	void draw(SDL_Renderer& renderer);
	void handleInput(InputSystem& input);

private:
	void handleButtonPressed();
	void createLobbyButtons();
	Entity* createButton(Vector2f pos, int index, std::string btnTag, int noOfPlayers, bool passProtected, bool selected);
	void requestHost();

	TTF_Font * m_font;
	SDL_Rect m_textRect;
	SDL_Color  * m_textColour;
	SDL_Texture * m_textTexture;

	OnlineSystem* m_network;

	int m_currentIndex;
	Camera m_camera;
	InputComponent* m_input;
	bool m_addedInput;

	vector<OnlineSystem::LobbyInfo> m_lobbies;

	std::vector<Entity*> m_buttons;
	Entity m_selectedBar;

	Entity m_bg;
};