#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include <stdio.h>


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

    /*l.SetPos(1, 30, 65);
    l.Active(true);
    l.ambient = Red;
    l.diffuse = Blue;*/

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	MapCreation();

    App->audio->PlayMusic("Assets/Audio/nine_thou.ogg");

    return ret;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

    display(dt);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MapCreation()
{
       
}

void ModuleSceneIntro::display(float dt)
{

}

void ModuleSceneIntro::ResetScene()
{
    
}