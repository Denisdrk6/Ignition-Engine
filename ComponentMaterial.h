#pragma once

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() { type = ComponentType::MATERIAL; };
	~ComponentMaterial() {};

	void Enable() {};
	void Update() {};
	void Disable() {};
};
