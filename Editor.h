#ifndef __Editor_H__
#define __Editor_H__

#include "Module.h"
#include "SDL/include/SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

class Application;

class Tab
{
public:
	Tab(bool active) { this->active = active; }
	Tab() { this->active = true; }

	void SwitchActive() { active = !active; }

private:
	bool active;
};

class Editor : public Module
{
public:

	Editor(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~Editor();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	
private:
	Tab* about;

	// Window options
	bool fullscreen = false;
	bool fullscreenDesk = false;
	bool resizable=true;

	// Sliders data
	float brightness=1.0f;
	int width=1280;
	int maxWidth=1500;
	int height=1024;
	int maxHeight=1500;

	// Our state
	bool toolMenu = false;
	bool showcaseDemo = false;
	bool show_another_window = false;
	bool config = false;
};

#endif // __Editor_H__