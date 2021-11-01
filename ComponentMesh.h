#pragma once

#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh() { type = ComponentType::MESH; };
	~ComponentMesh() {};

	void Enable() {};
	void Update() {};
	void Disable() {};
};
