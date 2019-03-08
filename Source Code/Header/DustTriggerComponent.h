#pragma once
#include "Component.h"

class DustTriggerComponent : public Component
{
public:
	DustTriggerComponent() :
		m_create(false)
	{}

	void setCreate() { m_create = true; }

	bool& toCreate() { return m_create; }
private:
	bool m_create;
};