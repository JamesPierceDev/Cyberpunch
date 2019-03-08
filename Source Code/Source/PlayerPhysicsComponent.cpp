#include "PlayerPhysicsComponent.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(Component * pos) :
	m_body(nullptr),
	m_jumpSensor(nullptr),
	m_stunned(false),
	m_canFall(false),
	m_canJump(false),
	m_movingL(true),
	m_movingR(false),
	m_gravFlipped(false),
	m_falling(false),
	m_supered(false),
	m_stunnedBySuper(false),
	m_setStatic(false),
	m_setDynamic(false),
	m_stunLeft(0),
	m_jumpSpeed(40.0f),
	m_jumpDownSpeed(20.0f),
	m_moveSpeed(10),
	m_dmgPercentage(0),
	m_originalMass(0),
	m_superPercentage(0)
{
	posPtr = static_cast<PositionComponent*>(pos);
}

void PlayerPhysicsComponent::stun()
{
	//Only do regular stun if we arent already stunned by a super
	if (m_stunnedBySuper == false)
	{
		//Set stunned to true
		m_stunned = true;

		//get the multiplier for a stun
		float stunMultiplier = m_dmgPercentage / 100.0f;

		//Multiply our stun left by our stun multiplier
		m_stunLeft = .3f * stunMultiplier;
	}
}

void PlayerPhysicsComponent::superStun()
{
	//Set stunned to true
	m_stunned = true;

	//Set stunned by super
	m_stunnedBySuper = true;

	//Reset the super impulse
	m_superImpulse = Vector2f();

	//Set stun to 5.0f
	m_stunLeft = 5.0f; 

	m_setStatic = true; //Make the body static
}

void PlayerPhysicsComponent::jump()
{
	m_desiredVel.y = -1;
}

void PlayerPhysicsComponent::jumpDown()
{
	m_desiredVel.y = 1;
	//Set the body as falling
	m_falling = true;
}

void PlayerPhysicsComponent::moveLeft()
{
	m_desiredVel.x = -1;
	m_movingL = true;
	m_movingR = false;
}

void PlayerPhysicsComponent::moveRight()
{
	m_desiredVel.x = 1;
	m_movingR = true;
	m_movingL = false;
}

void PlayerPhysicsComponent::addSuper(int amount)
{
	m_superPercentage += amount;

	m_superPercentage = m_superPercentage > 100 ? 100 : m_superPercentage;
}

void PlayerPhysicsComponent::changeBodyType(b2BodyType type)
{
	if (m_originalMass == 0)
		m_originalMass = m_body->getBody()->GetMass();

	b2MassData data;
	m_body->getBody()->GetMassData(&data);

	if (type == b2BodyType::b2_staticBody)
		data.mass = 99;
	else
		data.mass = m_originalMass;

	m_body->getBody()->SetMassData(&data);
}

void PlayerPhysicsComponent::applyDamageImpulse(float x, float y)
{
	auto imp = Vector2f(x, y);
	//Get the damage multiplier
	float dmgMultiplier = m_dmgPercentage / 100.0f;
	auto impulse = Vector2f(x < 0 ? -1 : 1, .75f);
	impulse = impulse.normalise() * ((impulse * imp) * dmgMultiplier);

	//If stunne dby a super, build up the impulse
	if (m_stunnedBySuper)
	{
		m_superImpulse += Vector2f(impulse.x * .5f, impulse.y * .9f);
	}
	else //If not hit by a super stun, apply knockback
	{
		//Apply knockback
		m_body->getBody()->ApplyLinearImpulse(b2Vec2(impulse.x, -impulse.y), m_body->getBody()->GetWorldCenter(), true);
	}
}

void PlayerPhysicsComponent::flipGravity(Box2DBridge& world)
{
	m_gravFlipped = !m_gravFlipped;
	m_sensorJointDef.localAnchorA.Set(m_sensorJoint->GetLocalAnchorA().x, -m_sensorJoint->GetLocalAnchorA().y);
	world.getWorld().DestroyJoint(m_sensorJoint);
	world.getWorld().CreateJoint(&m_sensorJointDef);
}

void PlayerPhysicsComponent::createJoint(Box2DBridge & world)
{
	//Define the sensor
	m_sensorJointDef.bodyA = m_body->getBody();
	m_sensorJointDef.bodyB = m_jumpSensor->getBody();
	m_sensorJointDef.collideConnected = false;
	m_sensorJointDef.localAnchorA.Set(0, 1.35f);

	//Create the joints
	m_sensorJoint = (b2RevoluteJoint*)world.getWorld().CreateJoint(&m_sensorJointDef);
}

float PlayerPhysicsComponent::clamp(float min, float & val, float max)
{
	val < min ? val = min : val;
	val > max ? val = max : val;

	return val;
}

void PlayerPhysicsComponent::move(int direction)
{
	m_currentVel.x += 0.4f * direction;

	m_currentVel.x = clamp(-m_moveSpeed, m_currentVel.x, m_moveSpeed);
}

void PlayerPhysicsComponent::moveUp()
{
	m_currentVel.y = 0;
	m_currentVel.y -= m_gravFlipped ? -m_jumpSpeed : m_jumpSpeed;
	m_canJump = false;
}

void PlayerPhysicsComponent::moveDown()
{
	m_currentVel.y = 0;
	m_currentVel.y -= m_gravFlipped ? m_jumpDownSpeed : -m_jumpDownSpeed;
	m_falling = true;
}

void PlayerPhysicsComponent::beginSuper()
{
	m_supered = true;
	m_superTime = 5.0f;
}

void PlayerPhysicsComponent::endSuper()
{
	m_supered = false;
	m_superTime = 0;
	m_superPercentage = 0;
}

void PlayerPhysicsComponent::endSuperStun()
{
	m_stunned = false;
	m_stunnedBySuper = false;

	m_setDynamic = true;

	//Stun the player after being hit
	stun();
	
	//Apply the impulse that has been built up by while super stunned
	applyDamageImpulse(m_superImpulse.x, m_superImpulse.y);
}
