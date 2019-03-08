#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <map>

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	std::string m_ID;
};

#endif