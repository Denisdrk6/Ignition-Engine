#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Editor.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	MYLOG("Init SDL window & surface");
	bool ret = true;

	title = App->appTitle + " - " + App->orgName;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	// Config file stuff
	if (resizable) window_flags = (SDL_WindowFlags)(window_flags | SDL_WINDOW_RESIZABLE);
	if (borderless) window_flags = (SDL_WindowFlags)(window_flags | SDL_WINDOW_BORDERLESS);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
	
	// More config file stuff
	SDL_SetWindowBrightness(window, brightness);

	return ret;
}

update_status ModuleWindow::Update(float dt)
{
	// Check if we are closing the window
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return update_status::UPDATE_STOP;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			return update_status::UPDATE_STOP;
	}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	MYLOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle()
{
	title = App->appTitle + " - " + App->orgName;
	SDL_SetWindowTitle(window, title.c_str());
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void ModuleWindow::SetFullscreenDesk(bool fullscreenDesk)
{
	SDL_SetWindowFullscreen(window, fullscreenDesk ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void ModuleWindow::SetBorder(bool borderless)
{
	SDL_SetWindowBordered(window, (SDL_bool)borderless);
}

void ModuleWindow::SetSize(int w, int h)
{
	SDL_SetWindowSize(window, w, h);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}
