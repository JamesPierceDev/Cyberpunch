#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
class Entity
{
public:
	Entity(std::string id) : m_ID(id) {};
	void addComponent(std::string id, Component* c) { m_components[id] = c; }
	void removeComponent(std::string id) { m_components.erase(id); }
	Component& getComponent(std::string id) { return *m_components[id]; }

	std::map<std::string, Component*> m_components; //Collection of components
	std::string m_ID; //ID of the entitiy
private:
};

#endif