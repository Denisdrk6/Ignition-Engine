#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "Math/float3.h"
#include "SDL_opengl.h"

#define MAX_LIGHTS 8

// Si solo usamos punteros a MeshStorage es mas eficiente hacer un forward declaration que incluir el FBXLoader.h
struct MeshStorage;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool DrawMesh(MeshStorage);

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext gl_context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool show_demo_window = true;
	bool show_another_window = false;

	// Config vars
	bool vsync = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

	bool drawNormals = false;

	bool wireframe = true;
};