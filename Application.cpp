#include "Application.h"
#include <iostream>
#include <fstream>

// Include Modules
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Editor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "FbxLoader.h"
#include "FileSystem.h"

Application::Application()
{
	log = new Logger();
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	editor = new Editor(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	fbx = new FbxLoader(this);
	fileSystem = new FileSystem(this,true, "/Assets/");

	log->AddLog("\n-------------- Application Creation --------------\n");

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fileSystem);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(physics);
	AddModule(fbx);
	
	// Scene
	AddModule(scene_intro);

	// Editor over scene
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


	std::ifstream fin;
	fin.open(CONFIG_FILNAME, std::ios::binary);
	if (!fin.fail()) LoadConfig();
	else SaveConfig();

	log->AddLog("INITIALIZING MODULES\n");

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
	log->AddLog("\n-------------- Application Start --------------\n");
	log->AddLog("Starting modules\n");
	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == true)
	{
		item = *iterator;
		ret = item->Start();
		iterator++;
	}
	
	ms_timer.Start();

	SDL_GetVersion(&SDLversion);
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

	// Erase last ", "
	int l = caps.size();
	caps.erase(l - 2, l - 1);

	gpuIntegratedVendor = (const char*)glGetString(GL_VENDOR);
	gpuIntegratedModel = (const char*)glGetString(GL_RENDERER);

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &GLverMajor);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &GLverMinor);

	glewVersion = (const char*)glewGetString(GLEW_VERSION);

	log->AddLog("Using Glew %s\n", glewVersion.c_str());
	log->AddLog("Using OpenGL %d.%d\n", GLverMajor, GLverMinor);

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

	SaveConfig(); // We should call it only if there's been changes

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

// Load config from XML file
bool Application::LoadConfig()
{
	bool ret = true;

	// TODO 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILNAME);

	// TODO 3: Check result for loading errors
	if (result == NULL)
	{
		log->AddLog("[error] Could not load config xml file. pugi error: %s\n", result.description());
		ret = false;
	}
	else
	{
		log->AddLog("Loading configuration\n");

		config = configFile.child("config");

		// Load app config
		pugi::xml_node app = config.child("App");
		appTitle = app.attribute("name").as_string();
		orgName = app.attribute("org").as_string();
		maxFPS = app.attribute("maxFPS").as_int();

		// Load window config
		pugi::xml_node windowNode = config.child("Window");
		window->width = windowNode.attribute("w").as_int();
		window->height = windowNode.attribute("h").as_int();
		window->brightness = windowNode.attribute("brightness").as_float();
		window->fullscreen = windowNode.attribute("fullscreen").as_bool();
		window->fullscreenDesk = windowNode.attribute("fulldesk").as_bool();
		window->resizable = windowNode.attribute("resizable").as_bool();
		window->borderless = windowNode.attribute("borderless").as_bool();

		// Load renderer config
		pugi::xml_node rendererNode = config.child("Renderer");
		renderer3D->vsync = rendererNode.attribute("vsync").as_bool();
		renderer3D->drawNormals = rendererNode.attribute("normals").as_bool();
		renderer3D->wireframe = rendererNode.attribute("wireframe").as_bool();

		// Load camera config
		pugi::xml_node cameraNode = config.child("Camera");
		pugi::xml_node pos = cameraNode.child("Position");
		camera->Position.x = pos.attribute("x").as_float();
		camera->Position.y = pos.attribute("y").as_float();
		camera->Position.z = pos.attribute("z").as_float();
		pugi::xml_node ref = cameraNode.child("Reference");
		camera->Reference.x = ref.attribute("x").as_float();
		camera->Reference.y = ref.attribute("y").as_float();
		camera->Reference.z = ref.attribute("z").as_float();
		pugi::xml_node X = cameraNode.child("ArcballX");
		camera->X.x = X.attribute("x").as_float();
		camera->X.y = X.attribute("y").as_float();
		camera->X.z = X.attribute("z").as_float();
		pugi::xml_node Y = cameraNode.child("ArcballY");
		camera->Y.x = Y.attribute("x").as_float();
		camera->Y.y = Y.attribute("y").as_float();
		camera->Y.z = Y.attribute("z").as_float();
		pugi::xml_node Z = cameraNode.child("ArcballZ");
		camera->Z.x = Z.attribute("x").as_float();
		camera->Z.y = Z.attribute("y").as_float();
		camera->Z.z = Z.attribute("z").as_float();

		// Load OpenGL config
		pugi::xml_node openGL = config.child("OpenGL");
		GLdepthTest = openGL.attribute("depthTest").as_bool();
		GLcullFace = openGL.attribute("cullFace").as_bool();
		GLlightning = openGL.attribute("lightning").as_bool();
		GLcolorMaterial = openGL.attribute("colorMaterial").as_bool();
		GLtexture2D = openGL.attribute("texture2D").as_bool();
		GLlineSmooth = openGL.attribute("lineSmooth").as_bool();
		pugi::xml_node fog = openGL.child("Fog");
		GLfog = fog.attribute("active").as_bool();
		fogLinear = fog.attribute("linear").as_bool();
		fogExpo = fog.attribute("expo").as_bool();
		fogR = fog.attribute("red").as_float();
		fogG = fog.attribute("green").as_float();
		fogB = fog.attribute("blue").as_float();
		fogStart = fog.attribute("start").as_float();
		fogEnd = fog.attribute("end").as_float();
		fogDensity = fog.attribute("density").as_float();
	}

	return ret;
}

bool Application::SaveConfig()
{
	bool ret = true;

	log->AddLog("Saving configuration in %s\n", CONFIG_FILNAME);

	pugi::xml_document file;
	pugi::xml_node base = file.append_child("config");

	//APPLICATION
	// App save
	pugi::xml_node app = base.append_child("App");
	app.append_attribute("name") = appTitle.c_str();
	app.append_attribute("org") = orgName.c_str();
	app.append_attribute("maxFPS") = maxFPS;

	//MODULES
	// Window save
	pugi::xml_node windowNode = base.append_child("Window");
	windowNode.append_attribute("w") = window->width;
	windowNode.append_attribute("h") = window->height;
	windowNode.append_attribute("brightness") = window->brightness;
	windowNode.append_attribute("fullscreen") = window->fullscreen;
	windowNode.append_attribute("fulldesk") = window->fullscreenDesk;
	windowNode.append_attribute("resizable") = window->resizable;
	windowNode.append_attribute("borderless") = window->borderless;

	// Renderer save
	pugi::xml_node rendererNode = base.append_child("Renderer");
	rendererNode.append_attribute("vsync") = renderer3D->vsync;
	rendererNode.append_attribute("normals") = renderer3D->drawNormals;
	rendererNode.append_attribute("wireframe") = renderer3D->wireframe;

	// Camera save

	pugi::xml_node cameraNode = base.append_child("Camera");

	pugi::xml_node pos = cameraNode.append_child("Position");
	pos.append_attribute("x") = camera->Position.x;
	pos.append_attribute("y") = camera->Position.y;
	pos.append_attribute("z") = camera->Position.z;

	pugi::xml_node ref = cameraNode.append_child("Reference");
	ref.append_attribute("x") = camera->Reference.x;
	ref.append_attribute("y") = camera->Reference.y;
	ref.append_attribute("z") = camera->Reference.z;

	pugi::xml_node X = cameraNode.append_child("ArcballX");
	X.append_attribute("x") = camera->X.x;
	X.append_attribute("y") = camera->X.y;
	X.append_attribute("z") = camera->X.z;
	pugi::xml_node Y = cameraNode.append_child("ArcballY");
	Y.append_attribute("x") = camera->Y.x;
	Y.append_attribute("y") = camera->Y.y;
	Y.append_attribute("z") = camera->Y.z;
	pugi::xml_node Z = cameraNode.append_child("ArcballZ");
	Z.append_attribute("x") = camera->Z.x;
	Z.append_attribute("y") = camera->Z.y;
	Z.append_attribute("z") = camera->Z.z;


	//GRAPHICS
	//OpenGL
	pugi::xml_node openGL = base.append_child("OpenGL");
	openGL.append_attribute("depthTest") = GLdepthTest;
	openGL.append_attribute("cullFace") = GLcullFace;
	openGL.append_attribute("lightning") = GLlightning;
	openGL.append_attribute("colorMaterial") = GLcolorMaterial;
	openGL.append_attribute("texture2D") = GLtexture2D;
	openGL.append_attribute("lineSmooth") = GLlineSmooth;
	pugi::xml_node fog = openGL.append_child("Fog");
	fog.append_attribute("active") = GLfog;
	fog.append_attribute("linear") = fogLinear;
	fog.append_attribute("expo") = fogExpo;
	fog.append_attribute("red") = fogR;
	fog.append_attribute("green") = fogG;
	fog.append_attribute("blue") = fogB;
	fog.append_attribute("start") = fogStart;
	fog.append_attribute("end") = fogEnd;
	fog.append_attribute("density") = fogDensity;

	bool succ = file.save_file(CONFIG_FILNAME);
	if (succ != true)
	{
		log->AddLog("Config file save error. pugi error: %d\n", pugi::status_internal_error);
	}
	else log->AddLog("... finished saving\n");

	return ret;
}