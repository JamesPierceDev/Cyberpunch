#include "AnimationSystem.h"

void AnimationSystem::addComponent(Component * comp)
{
	m_components.push_back(comp);
}

void AnimationSystem::update(double dt)
{
	for (auto c : m_components)
	{
		//Convert the component to an animation component
		auto aComp = dynamic_cast<AnimationComponent*>(c);

		auto anim = aComp->getCurrentAnimation();

		//Only run if the pointer is not null
		if (nullptr != anim)
		{
			//Only run if the animation is not completed
			if (anim->getCompleted() == false)
			{
				//Add dt to time gone
				anim->getTimeGone() += dt;

				//If the time gone is now passed the time per frame
				if (anim->getTimeGone() >= anim->getTimePerFrame())
				{
					//Increase the current frame
					anim->getCurrentFrame()++;

					//Subtract the time per frame from the time gone
					anim->getTimeGone() -= anim->getTimePerFrame();

					//If the current frames has reached the max frames then end the animation
					if (anim->getCurrentFrame() >= anim->getMaxFrames())
					{
						//If the animation is looping
						if (anim->getLoop())
						{
							anim->getCurrentFrame() = 0; //Reset the current frame
						}
						//Else if loop is false
						else
						{
							//Set the max frame to the last frame of the animation
							anim->getCurrentFrame() = anim->getMaxFrames() - 1;

							//Set animation as completed
							anim->getCompleted() = true;
						}
					}

					//Set current frame of the sprite
					aComp->getSprite()->setTextureRect(anim->getCurrentTextureRect());
				}
			}
		}
	}
}
