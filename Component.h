#pragma once
#include "GameObject.h"

enum class ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component {
public:
	Component() {};
	~Component() {};

	ComponentType type = ComponentType::TRANSFORM;
	bool active = true;
	GameObject* owner = nullptr;

	virtual void Enable() { active = true; };
	virtual void Update() {};
	virtual void Disable() { active = false; };
};