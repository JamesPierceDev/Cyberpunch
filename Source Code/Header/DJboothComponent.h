#pragma once
#include "Component.h"
#include "iostream"
#include "Observer.h"

class DJBoothComponent : public Component, public Subject
{
public: 
	DJBoothComponent(Entity* pickUp) : bgSwitch(false), m_pickUp(pickUp){}
	virtual void run() = 0;
	virtual void update(double dt) = 0;
	bool bgSwitch;
	Entity* m_pickUp;
private:
};