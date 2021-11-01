#pragma once

#include "Component.h"
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform() { type = ComponentType::TRANSFORM; };
	~ComponentTransform() {};

	void Enable() {};
	void Update() {};
	void Disable() {};

	Quat rotation;
	float3 position, scale;
	float4x4 matrix;
};