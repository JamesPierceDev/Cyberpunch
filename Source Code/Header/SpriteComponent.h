#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>
#include "PositionComponent.h"
#include "Vector2f.h"

class SpriteComponent : public Component
{
public:
	//Sprite component take sin the position component (where its placed), the overallSize of the sprite
	//the size of a single frame (this can be the same as overall size if its a 1 frame sprite)
	//the texture itself and the layer of the sprite
	SpriteComponent(Component* pos, Vector2f overallSize, Vector2f singleFramebounds, SDL_Texture * texture, int layer = 0):
		m_bounds(overallSize),
		m_angle(0),
		m_singleFrameBounds(singleFramebounds),
		m_texture(texture),
		m_layer(layer),
		m_posPtr(pos),
		m_scale(1,1), //Scale is 1, 1, sprite is not flipped
		m_flip(SDL_FLIP_NONE),
		m_useCamera(true)
	{
		m_src = { 0, 0, (int)singleFramebounds.x, (int)singleFramebounds.y };
		m_dst = { 0, 0, (int)singleFramebounds.x, (int)singleFramebounds.y };
	}

	//Setters
	void setAngle(float a) { m_angle = a; }
	void setTexture(SDL_Texture* texture) { m_texture = texture; }
	void setTextureRect(SDL_Rect frame) { m_src = frame; }
	void setScale(float x, float y)
	{
		//Sets the scale of the image
		m_scale = Vector2f(x,y);

		if (m_scale.x == -1 && m_scale.y == -1)
			m_flip = SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		else if(m_scale.y == -1)
			m_flip = SDL_RendererFlip(SDL_FLIP_VERTICAL);
		else if(m_scale.x == -1)
			m_flip = SDL_RendererFlip(SDL_FLIP_HORIZONTAL);
		else
			m_flip = SDL_RendererFlip(SDL_FLIP_NONE);
	}

	void setSourceRect(SDL_Rect size) { m_src = size; };
	void setDestRect(SDL_Rect size) { m_dst = size; }
	void setPosPtr(Component* pos) { m_posPtr = pos; }

	//Getters
	Vector2f& getPosition() { return static_cast<PositionComponent*>(m_posPtr)->position; }
	Vector2f getBounds() { return m_bounds; }
	Vector2f getFrameSize() { return m_singleFrameBounds; }
	Vector2f getScale() { return m_scale; }
	SDL_Texture* getTexture() { return m_texture; }
	SDL_Rect getSourceRect() { return m_src; }
	SDL_Rect getDestRect() { return m_dst; }
	SDL_RendererFlip& getFlip() { return m_flip; }
	int getLayer() { return m_layer; }
	float& getAngle() { return m_angle; }
	bool& useCamera() { return m_useCamera; }
private:
	Vector2f m_scale;
	SDL_RendererFlip m_flip;
	Component* m_posPtr;
	Vector2f m_bounds, m_singleFrameBounds;
	SDL_Rect m_src;
	SDL_Rect m_dst;
	SDL_Texture * m_texture;
	int m_layer;
	float m_angle;
	bool m_useCamera;
};

#endif