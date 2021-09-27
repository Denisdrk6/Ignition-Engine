#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
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

	// Renderer last!
	AddModule(renderer3D);
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
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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