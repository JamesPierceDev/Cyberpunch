#pragma once
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "System.h"
#include "InputComponent.h"

class InputSystem : public System
{
public: 
	InputSystem() {}
	~InputSystem() {}
	void addComponent(Component * component);
	void update(double dt);
};

#endif
