#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	editor = new Editor(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);

	ms_log.resize(60);
	fps_log.resize(60);
}

Application::~Application()
{
	std::list<Module*>::iterator iterator = list_modules.end();
	iterator--;
	Module* item;

	while (iterator != list_modules.begin())
	{
		item = *iterator;
		iterator--;
		delete item;
	}

	// Call last module delete (it's not called on the loop)
	item = *iterator;
	delete item;
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator iterator = list_modules.begin();
	Module* item;

	while(iterator != list_modules.end() && ret == true)
	{
		item = *iterator;
		ret = item->Init();
		iterator++;
	}

	// After all Init calls we call Start() in all modules
	MYLOG("Application Start --------------");
	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == true)
	{
		item = *iterator;
		ret = item->Start();
		iterator++;
	}
	
	ms_timer.Start();

	SDL_GetVersion(&ver);
	cpuCount = SDL_GetCPUCount();
	cpuCacheSize = SDL_GetCPUCacheLineSize();
	ram = SDL_GetSystemRAM() / 1000;

	if (SDL_HasAVX())
		caps += "AVX, ";
	if (SDL_HasAVX2())
		caps += "AVX2, ";
	if (SDL_HasMMX())
		caps += "MMX, ";
	if (SDL_HasRDTSC())
		caps += "RDTSC, ";
	if (SDL_HasSSE())
		caps += "SSE, ";
	if (SDL_HasSSE2())
		caps += "SSE2, ";
	if (SDL_HasSSE3())
		caps += "SSE3, ";
	if (SDL_HasSSE41())
		caps += "SSE41, ";
	if (SDL_HasSSE42())
		caps += "SSE42, ";

	int l = caps.size();
	caps.erase(l - 1, l);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	float ms = (float)ms_timer.Read();

	ms_timer.Start();

	if(ms_log[59] != 0.0f)
	{
		std::rotate(ms_log.begin(), ms_log.begin() + 1, ms_log.end());
		std::rotate(fps_log.begin(), fps_log.begin() + 1, fps_log.end());
	}

	ms_log.at(vectorAllocator) = ms;
	fps_log.at(vectorAllocator) = (int)(1000.0f / ms);
	//fps_log.at(vectorAllocator) = maxFPS;

	if (vectorAllocator < 59) vectorAllocator++;

	dt = ms / 1000.0f;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float maxMsPerFrame = (1000.0f / maxFPS); // Calculate max ms per frame
	float thisFrameMs = ms_timer.Read();

	if (thisFrameMs < maxMsPerFrame)
		SDL_Delay((Uint32)(maxMsPerFrame - thisFrameMs)); // Delay to cap framerate
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator iterator = list_modules.begin();
	Module* item;

	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->PreUpdate(dt);
		iterator++;
	}

	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->Update(dt);
		iterator++;
	}

	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->PostUpdate(dt);
		iterator++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator iterator = list_modules.end();
	iterator--;
	Module* item;

	while(iterator != list_modules.begin() && ret == true)
	{
		item = *iterator;
		ret = item->CleanUp();
		iterator--;
	}

	// Call last module cleanup (it's not called on the loop)
	item = *iterator;
	ret = item->CleanUp();

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RequestBrowser(const char* link)
{
	ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}