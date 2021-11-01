#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Globals.h"

GameObject::GameObject()
{
	transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);
	components.push_back(transform);
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
	case ComponentType::MESH:
		component = new ComponentMesh();
		component->owner = this;
		break;
	case ComponentType::MATERIAL:
		component = new ComponentMaterial();
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