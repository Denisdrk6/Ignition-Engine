#pragma once

#include <string>
#include <vector>
//#include "Component.h"
class Component;
enum class ComponentType;
class ComponentTransform;

class GameObject {
public:
	GameObject();
	~GameObject() {};

	void Update();
	void CleanUp();

	Component* CreateComponent(ComponentType type);

	bool active = true;
	std::string name = "\0";
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	ComponentTransform* transform;
};