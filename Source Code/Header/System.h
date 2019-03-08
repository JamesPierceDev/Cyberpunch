#ifndef SYSTEM_H
#define SYSTEM_H

#include <algorithm>
#include "Component.h"
#include <iostream>

class System
{
public:
	System() {};
	virtual ~System() {}
	virtual void addComponent(Component *) = 0;
	virtual void update(double dt) = 0;

	//Delect component method
	void deleteComponent(Component* comp)
	{
		m_components.erase(std::remove(m_components.begin(), m_components.end(), comp), m_components.end());
	}

	void removeAllComponents()
	{
		m_components.clear();
	}

	std::vector<Component *> m_components;
};

#endif