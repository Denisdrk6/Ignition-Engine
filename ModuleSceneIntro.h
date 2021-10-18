#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float3.h"

#include "SDL_opengl.h"
//#pragma comment( lib, "SDL/libx86/SDL2.lib" )
//#pragma comment( lib, "SDL/libx86/SDL2main.lib" )


#include <stdio.h>

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
    float3 v0 = float3(0.5f, 0.5f, 0.5f);
    float3 v1 = float3(-0.5f, 0.5f, 0.5f);
    float3 v2 = float3(-0.5f, -0.5f, 0.5f);
    float3 v3 = float3(0.5f, -0.5f, 0.5f);
    float3 v4 = float3(0.5f, -0.5f, -0.5f);
    float3 v5 = float3(0.5f, 0.5f, -0.5f);
    float3 v6 = float3(-0.5f, 0.5f, -0.5f);
    float3 v7 = float3(-0.5f, -0.5f, -0.5f);

    uint my_id = 0;
    const int num_vertices = 8;
    float3 vertices[8] = { v0, v1, v2, v3, v4, v5, v6, v7 };

    uint my_indices = 0;
    const int num_indices = 36;
    uint indices[36] = { 0, 1, 2, 2, 3, 0, 0, 3, 4, 4, 5, 0, 6, 1, 0, 0, 5, 6, 5, 4, 7, 7, 6, 5, 1, 6, 7, 7, 2, 1, 4, 3, 2, 2, 7, 4 };
};
