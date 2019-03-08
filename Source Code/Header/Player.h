#ifndef PLAYER_H
#define PLAYER_H

#include "Box2DBridge.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "SDL.h"

class Player {

public:
	Player();

	void createPlayer(Box2DBridge& world, PhysicsSystem& system);
	void deletePlayer();
	void update(double dt);
	void offsetAttackSensor();
	void checkForPunch(double dt);
	void checkForKick(double dt);
	void checkToDeleteAttack(double dt);
	void draw(SDL_Renderer& renderer);
	void handleInput(InputSystem& input);

	void addClient();

	void flipGravity();
	void jump();
	void jumpDown();
	void moveLeft();
	void moveRight();
	void punch();
	void kick();
	void punchUp();
	void punchDown();
	void upperCut();
	void stun();
	void super();
	void damage(int percent);
	void applyDmgImpulse(float x, float y);
	void deleteAttackBody();

	void setCanJump(bool b) { m_canJump = b; }
	void setCanFall(bool b) { m_canFall = b; }
	bool& falling() { return m_falling; }
	bool& punched() { return m_punched; } //Return wheter the player punched or not
	bool& punchedDown() { return m_punchedDown; } //Return wheter the player punched down
	bool& punchedUp() { return m_punchedUp; } //Return wheter the player punched up
	bool& uppercut() { return m_upperCut; }
	bool& kicked() { return m_kicked; } //Return wheter the player kicked or not
	bool& stunned() { return m_stunned; } //Return wheter the player is stunned or not
	bool attacked() { return !m_canAttack; } //Return th eopposie of can attack (if we cant attack, we have attacked)
private:
	//Private methods
	void attack(std::string tag, PhysicsComponent& sensor, int sizeX, int sizeY);

	int m_dmgPercent;
	b2Vec2 m_currentVel, m_desiredVel;
	Vector2f m_attackBoxOffset; //The offset of the attack box
	float m_moveSpeed;
	float m_jumpSpeed, m_jumpDownSpeed;
	bool m_canJump, m_gravFlipped, m_movingL, m_movingR;
	SDL_Rect m_rect;

	//OnlineSendComponent* m_client;
	bool is_online = false;

	PhysicsSystem* m_physPtr;
	PhysicsComponent m_physComponent, m_floorSensor;
	PositionComponent m_posComponent;

	//Attacking variables
	PhysicsComponent m_leftAttackSensor, m_rightAttackSensor, m_upAttackSensor, m_downAttackSensor, m_upperCutSensor;
	//Current attacking sensor
	PhysicsComponent* m_currentAttackSensor;

	Box2DBridge* m_worldPtr;

	b2RevoluteJoint * m_sensorJoint;
	b2RevoluteJointDef m_sensorJointDef;

	//Attack variables
	float m_punchRecharge, m_timeTillPunch, m_kickRecharge, m_timeTillKick, m_lpttl, m_rpttl, m_puttl, m_stunLeft;
	bool m_canPunch, m_punched, m_kicked, m_stunned, m_canAttack, m_punchedUp, m_punchedDown, m_upperCut;

	//Bools to determine if the player can flick down on the joycon stick
	bool m_canFall, m_falling;
};

#endif
