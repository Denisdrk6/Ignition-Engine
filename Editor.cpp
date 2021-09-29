#include "Globals.h"
#include "Application.h"
#include "Editor.h"

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	about = new Tab(true);
}

// Destructor
Editor::~Editor()
{
}

// Called before render is available
bool Editor::Init()
{
	bool ret = true;

	return ret;
}

bool Editor::Start()
{
	bool ret = true;
	// Setup SDL
	// (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
	// depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	return ret;
}

update_status Editor::Update(float dt)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showcaseDemo)
		ImGui::ShowDemoWindow(&showcaseDemo);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showcaseDemo);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&App->renderer3D->clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	//ImGui::Begin("Tool Bar", &toolMenu, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	//ImGui::BeginMainMenuBar();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Examples"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("GuiDemo")) showcaseDemo = !showcaseDemo;

			if (ImGui::MenuItem("Documentation")) App->RequestBrowser("https://github.com/d0n3val/Edu-Game-Engine/wiki");

			if (ImGui::MenuItem("Download latest")) App->RequestBrowser("https://github.com/d0n3val/Edu-Game-Engine/releases");

			if (ImGui::MenuItem("Report a bug")) App->RequestBrowser("https://github.com/d0n3val/Edu-Game-Engine/issues");

			if (ImGui::MenuItem("About")) about->SwitchActive();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//ImGui::EndMainMenuBar();

	//ImGui::End();
	

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool Editor::CleanUp()
{
	delete about;
	about = nullptr;

	return true;
}