#pragma once
#include "Component.h"

class DustComponent : public Component
{
public:
	DustComponent() :
		m_ttl(.5f),
		m_destroy(false)
	{

	}

	bool& toDestroy() { return m_destroy; }
	float& getTTL() { return m_ttl; }
private:
	bool m_destroy;
	float m_ttl;
};