#include "Box2DBridge.h"

Box2DBridge::Box2DBridge() :
	m_gravFlipped(false)
{
}

void Box2DBridge::initWorld()
{
	m_world = new b2World(GRAVITY); //Create the world
	m_world->SetGravity(GRAVITY); //Set the gravity of the world
	m_world->SetContinuousPhysics(true);
}

void Box2DBridge::update(double dt)
{
	//If there are bodies to delete, delete them
	if (!m_bodiesToDelete.empty())
	{
		for (auto& body : m_bodiesToDelete)
		{
			m_world->DestroyBody(body->getBody());
		}
		m_bodiesToDelete.clear();
	}
	m_timeSinceLastFrame += dt;
	if (m_timeSinceLastFrame > m_secondsPerFrame)
	{
		//Simulate the physics bodies
		m_world->Step(m_timeSinceLastFrame, VELOCITY_ITERS, POSITION_ITERS); 
		m_timeSinceLastFrame = 0;
	}
}

void Box2DBridge::flipGravity()
{
	m_gravFlipped = !m_gravFlipped;

	//Set the gravity of the world based on the bool
	if(m_gravFlipped)
		m_world->SetGravity(FLIPPEDGRAVITY);
	else
		m_world->SetGravity(GRAVITY);
}

void Box2DBridge::addContactListener(CollisionListener & colListener)
{
	m_world->SetContactListener(&colListener);
}

void Box2DBridge::deleteBody(Box2DBody * body)
{
	m_bodiesToDelete.push_back(body);
}

void Box2DBridge::deleteWorld()
{
	delete m_world;
}

Box2DBody* Box2DBridge::createBox(int posX, int posY, int width, int height, bool canRotate, bool allowSleep, b2BodyType type)
{
	Box2DBody* body =  new Box2DBody();
	b2BodyDef bDef;
	b2FixtureDef fDef;
	b2PolygonShape box;

	//Set the body type and its position
	bDef.type = type;
	bDef.fixedRotation = !canRotate;
	bDef.allowSleep = allowSleep;
	box.SetAsBox((width / 2.0f) / CONVERSION, (height / 2.0f) / CONVERSION);
	bDef.position.Set(posX / CONVERSION, posY / CONVERSION);

	fDef.shape = &box;

	//Create the body and set it for our box2D body wrapper
	body->setBody(m_world->CreateBody(&bDef));

	body->getBody()->CreateFixture(&fDef);
	return body; //Return the body
}

Box2DBody* Box2DBridge::createCircle(int posX, int posY, float radius, bool canRotate, bool allowSleep, b2BodyType type)
{
	Box2DBody* body = new Box2DBody();
	b2BodyDef bDef;
	b2CircleShape circle;

	//Set the body type and its position
	bDef.type = type;
	bDef.fixedRotation = canRotate;
	bDef.allowSleep = allowSleep;
	circle.m_p.Set(posX, posY);
	circle.m_radius = radius;
	bDef.position.Set(posX, posY);

	body->setBody(m_world->CreateBody(&bDef));
	return body; //Return the body
}

void Box2DBridge::addProperties(Box2DBody & body, float mass, float friction, float rest, bool isSensor, void* data)
{
	//Get a pointer to the fixture for the body
	b2Fixture* fDef = body.getBody()->GetFixtureList();
	fDef->SetSensor(isSensor); //Set wheter the body is a sensor (doesnt take part of regular physics)
	fDef->SetDensity(mass); //Set the density/mass of the body
	fDef->SetFriction(friction); //Set friction of the body
	fDef->SetUserData(data); //Set the user data
	fDef->SetRestitution(rest); //Set the restitution of the body
}

void Box2DBody::setPosition(float x, float y)
{
	//Set the position of the physics body
	m_body->SetTransform(b2Vec2(x / CONVERSION, y / CONVERSION), 0);
}

//Box2DBody methods
Vector2f Box2DBody::getPosition()
{
	//Get the shape of the body
	auto size = getSize();
	m_position.x = (m_body->GetPosition().x * CONVERSION);
	m_position.y = (m_body->GetPosition().y * CONVERSION);

	//Return our position
	return m_position;
}

Vector2f Box2DBody::getSize()
{
	auto shape = m_body->GetFixtureList()->GetAABB(0);
	return Vector2f(shape.GetExtents().x * CONVERSION * 2, shape.GetExtents().y * CONVERSION * 2);
}


float Box2DBody::getAngle()
{
	return 0.0f;
}
