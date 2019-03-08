#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include "Camera.h"
#include "Scene.h"
#include "Entity.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "RenderSystem.h"

class OptionsScene : public Scene
{
public:
	OptionsScene();
	~OptionsScene();
	void start();
	void stop();
	void update(double dt);
	void draw(SDL_Renderer& renderer);
	void handleInput(InputSystem& input);
private:
	Camera m_camera;
	InputComponent* m_input;	
	Entity * m_txtImage;
	bool m_initialSetup;
};

#endif
