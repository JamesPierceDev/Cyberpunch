#include "PickUp.h"

PickUp::PickUp() :
	m_moveSpeed(5),
	m_posComonponent(800,800),
	m_physComponent(&m_posComonponent),
	m_draw(true)
{


}

void PickUp::createPickUp(Box2DBridge& world, PhysicsSystem& system)
{
	if (m_draw == true)
	{

		m_worldPtr = &world;
		physPtr = &system;
		m_physComponent = PhysicsComponent(&m_posComonponent);

		m_physComponent.m_body = world.createBox(950, 400, 40, 40, false, false, b2BodyType::b2_dynamicBody);
		world.addProperties(*m_physComponent.m_body, 1, 0.1f, 0.0f, true, new PhysicsComponent::ColData("Pick-Up", this));

		m_physComponent.m_body->getBody()->SetGravityScale(0.0f);

	}
}

void PickUp::deletePickUp()
{

	m_draw = false;
}

void PickUp::update(double dt)
{



}

void PickUp::draw(SDL_Renderer & renderer)
{
	if (m_draw == true)
	{
		m_pickUp.w = m_physComponent.m_body->getSize().x;
		m_pickUp.h = m_physComponent.m_body->getSize().y;
		m_pickUp.x = m_physComponent.m_body->getPosition().x - (m_pickUp.w / 2.0f);
		m_pickUp.y = m_physComponent.m_body->getPosition().y - (m_pickUp.h / 2.0f);

		SDL_SetRenderDrawColor(&renderer, 5, 255, 0, 255);
		SDL_RenderDrawRect(&renderer, &m_pickUp);
	}


}