#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreenDesk(bool fullscreenDesk);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBrightness(float brightness);
	void SetSize(int w, int h);
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	static char title;
};

#endif // __ModuleWindow_H__