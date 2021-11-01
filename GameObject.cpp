#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "Globals.h"

GameObject::GameObject()
{
	components.push_back(CreateComponent(ComponentType::TRANSFORM));
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component;


	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new ComponentTransform();
		component->owner = this;
		break;
	default:
		break;
	}

	return component;
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
		components.at(i)->Update();
}

void GameObject::CleanUp()
{
	for (int i = 0; i < components.size(); i++)
	{
		RELEASE(components.at(i));
	}

	components.clear();
}