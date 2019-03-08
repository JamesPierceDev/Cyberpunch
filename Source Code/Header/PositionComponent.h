#pragma once
#include "Component.h"
#include "Vector2f.h"

class PositionComponent : public Component
{
public:
	PositionComponent(float x, float y) : position(x, y) {}

	Vector2f position;
};