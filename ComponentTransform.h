#pragma once

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform() { type = ComponentType::TRANSFORM; };
	~ComponentTransform() {};

	void Enable() {};
	void Update() {};
	void Disable() {};
};