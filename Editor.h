#ifndef __Editor_H__
#define __Editor_H__

#include "Module.h"
#include "SDL/include/SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

class Application;

/*class Tab
{
public:
	Tab(bool active) { this->active = active; }
	Tab() { this->active = true; }

	void SwitchActive() { active = !active; }

private:
	bool active;
};*/

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
	//Tab* about;
	bool about = false;

	// Our state
	bool toolMenu = false;
	bool showcaseDemo = false;
	bool show_another_window = false;
	bool config = false;

	// App variables needed here so we can change window name
	char appTitle[64];
	char orgName[64];
};

#endif // __Editor_H__