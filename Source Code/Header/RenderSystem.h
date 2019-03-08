#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include "Camera.h"
#include <algorithm>
#include <SDL.h>
#include "SpriteComponent.h"

class RenderSystem : public System
{
public:
	RenderSystem();
	void update(double dt);
	void addComponent(Component *);
	void render(SDL_Renderer& renderer, Camera& camera);

private:
	SDL_Rect m_spritePos;

	//Struct used to sort the components based on their layer (smallest to biggest)
	struct layerSorter
	{
		inline bool operator() (Component* l, Component* r)
		{
			auto lhs = static_cast<SpriteComponent*>(l);
			auto rhs = static_cast<SpriteComponent*>(r);

			return lhs->getLayer() < rhs->getLayer();
		}
	};

};

#endif