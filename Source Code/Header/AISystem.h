#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "System.h"
#include "AIComponent.h"

class AISystem : public System 
{
public:
	void createTree();
	void runTree();
	void update(double dt);
	void addComponent(Component *);
private:
	float timer = 0;
};

#endif