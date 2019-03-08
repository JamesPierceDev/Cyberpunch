#pragma once
#include "Box2DBridge.h"
#include "PositionComponent.h"

class PlayerPhysicsComponent : public Component
{
public:
	PlayerPhysicsComponent(Component* pos);

	//methods for modifying the player
	void stun();
	void superStun();
	void jump();
	void jumpDown();
	void moveLeft();
	void moveRight();
	void move(int direction);
	void moveUp();
	void moveDown();
	void beginSuper();
	void endSuper();
	void endSuperStun();
	void damage(int dmg) {m_dmgPercentage += dmg; }
	void addSuper(int amount);
	void changeBodyType(b2BodyType type);
	void applyDamageImpulse(float x, float y);
	void flipGravity(Box2DBridge& world);
	void createJoint(Box2DBridge& world);

	//Getter methods
	bool& stunned() { return m_stunned; };
	bool& canJump() { return m_canJump; }
	bool& canFall() { return m_canFall; }
	bool& falling() { return m_falling; }
	bool& isGravityFlipped() { return m_gravFlipped; }
	bool& isMovingLeft() { return m_movingL; }
	bool& isMovingRight() { return m_movingR; }
	bool& superStunned() { return m_stunnedBySuper; }
	bool& isSupered() { return m_supered; }
	bool canSuperUp() { return m_superPercentage >= 100; }
	float& stunLeft() { return m_stunLeft; }
	float& superLeft() { return m_superTime; }
	bool& toSetStatic() { return m_setStatic; }
	bool& toSetDynamic() { return m_setDynamic; }
	int& damagePercentage() { return m_dmgPercentage; }
	int& superPercentage() { return m_superPercentage; }
	bool& isOnPlayer() { return m_onPlayer; }

	//Setters
	void setCanFall(bool b) { m_canFall = b; }
	void setCanJump(bool b) { m_canJump = b; }
	void setOnPlayer(bool b) { m_onPlayer = b; }

	//---Member variables---//
	//The body of the physics component
	Box2DBody* m_body;
	Box2DBody* m_jumpSensor;
	//Hold a ptr to the position component that the physics modifies
	PositionComponent * posPtr;
	b2Vec2 m_currentVel, m_desiredVel;
private:
	bool m_setStatic, m_setDynamic;
	float clamp(float min, float& val, float max);
	b2RevoluteJointDef m_sensorJointDef;
	b2RevoluteJoint * m_sensorJoint;
	bool m_falling, m_stunned, m_canJump, m_canFall, m_movingL, m_movingR, m_gravFlipped, m_supered, m_stunnedBySuper;
	float m_stunLeft, m_moveSpeed, m_jumpDownSpeed, m_jumpSpeed;
	float m_superTime;
	bool m_onPlayer = false;
	int m_dmgPercentage, m_superPercentage;
	float m_originalMass;
	Vector2f m_superImpulse; //The impulse applied to the player after a super is over
};