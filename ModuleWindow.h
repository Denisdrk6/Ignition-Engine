#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include <string>

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

	void SetTitle();
	void SetFullscreenDesk(bool fullscreenDesk);
	void SetFullscreen(bool fullscreen);
	void SetBrightness(float brightness);
	void SetSize(int w, int h);
	void SetBorder(bool borderless);
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	std::string title;
};

#endif // __ModuleWindow_H__