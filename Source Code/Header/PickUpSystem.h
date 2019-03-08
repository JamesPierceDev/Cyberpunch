#ifndef PICKUPSYSTEM_H
#define PICKUPSYSTEM_H
#include "System.h"
#include "PickUpComponent.h"
#include "OnlineSystem.h"

class PickUpSystem : public System
{
public:
	PickUpSystem(System* renderSys, OnlineSystem* netSys) :
		m_renderSysPtr(renderSys),
		m_netSysPtr(netSys)
	{}
	void setWorld(Box2DBridge& world);
	void addComponent(Component* comp);
	void update(double dt);
private:
	Box2DBridge * m_worldPtr;
	System* m_renderSysPtr;
	OnlineSystem* m_netSysPtr;
};

#endif