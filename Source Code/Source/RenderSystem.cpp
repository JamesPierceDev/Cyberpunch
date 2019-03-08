#include "../Header/RenderSystem.h"

RenderSystem::RenderSystem()
{
}

/// <summary>
/// Add a new component to the vector
/// in the renderSystem
/// </summary>
/// <param name="c"></param>
void RenderSystem::addComponent(Component * c)
{
	m_components.push_back(c);

	//Sort the sprite components based on their layers
	std::sort(m_components.begin(), m_components.end(), layerSorter());
}

/// <summary>
/// Uses SDL_RenderCopy to draw
/// object using parametres from 
/// the component class
/// </summary>
void RenderSystem::render(SDL_Renderer& renderer, Camera& camera)
{
	camera.setZoom(&renderer);

	//Loop through all 
	for (auto c : m_components)
	{
		//Convert component to a sprite
		auto sprite = dynamic_cast<SpriteComponent*>(c);

		m_spritePos = sprite->getDestRect(); //Can modify the position using the camera later

		//position of the destination rect using the position ptr
		m_spritePos.x = sprite->getPosition().x - (sprite->getFrameSize().x / 2);
		m_spritePos.y = sprite->getPosition().y - (sprite->getFrameSize().y / 2);

		//Minus camera when we do a camera
		if (sprite->useCamera())
		{
			m_spritePos.x -= camera.x();
			m_spritePos.y -= camera.y();
		}

		auto scalar = camera.getScale();
		if (sprite->useCamera() == false)
		{
			camera.scalar() = 1;
			camera.setZoom(&renderer);
		}

		//Draw the sprite
		SDL_RenderCopyEx(&renderer, sprite->getTexture(), &sprite->getSourceRect(), &m_spritePos, sprite->getAngle(), 0, sprite->getFlip());

		if (sprite->useCamera() == false)
		{
			camera.scalar() = scalar;
			camera.setZoom(&renderer);
		}
	}
}

/// <summary>
/// Loops through render components
/// creates SDL_Rects using their values
/// and calls render function.
/// </summary>
void RenderSystem::update(double dt)
{

}