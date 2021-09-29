#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Editor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <list>

#include "MathGeoLib.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	Editor* editor;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	
	SDL_GLContext gl_context;

	std::vector<float> fps_log;
	std::vector<float> ms_log;

	int maxFPS = 60.0f;

private:
	
	Timer	ms_timer;
	float	dt;
	float delayTime = 0.0f; //Used for capping fps

	std::list<Module*> list_modules;

	int vectorAllocator = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* link);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};