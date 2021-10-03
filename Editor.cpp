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


	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	maxWidth = DM.w;
	maxHeight = DM.h;

	return ret;
}

update_status Editor::Update(float dt)
{


	//ImGui::Begin("Tool Bar", &toolMenu, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	//ImGui::BeginMainMenuBar();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Configuration")) config = !config;

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

	if (config)
	{
		ImGui::Begin("Configuration", &config);
		ImGui::Text("Options");
		ImGuiTreeNodeFlags(ImGuiTreeNodeFlags_Framed);
		

		if (ImGui::CollapsingHeader("Application")) // App configuration
		{
			std::strcpy(appTitle, App->appTitle.c_str());
			ImGui::InputText("App name", appTitle, IM_ARRAYSIZE(appTitle));
			if (App->appTitle != (std::string)appTitle)
			{
				App->appTitle = appTitle;
				App->window->SetTitle();
			}


			std::strcpy(orgName, App->orgName.c_str());
			ImGui::InputText("Organization", orgName, IM_ARRAYSIZE(orgName));
			if (App->orgName != (std::string)orgName)
			{
				App->orgName = (const char*)orgName;
				App->window->SetTitle();
			}

			ImGui::SliderInt("Max FPS", &App->maxFPS, 1, 60);
			std::string str = std::to_string(App->maxFPS);
			ImGui::Text("Fps Limit: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%d.%d.%d", App->ver.major, App->ver.minor, App->ver.patch);
			ImGui::PopStyleColor();

			//ImGui::LabelText(str.c_str(), "CITM students");

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
			ImGui::PlotHistogram("##FRAMERATE", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Miliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
			ImGui::PlotHistogram("##MILISECONDS", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}

		if (ImGui::CollapsingHeader("Window")) // Window configuration
		{
			if (ImGui::SliderFloat("Brightness", &brightness, 0.1f, 1.0f)) // Brightness
				App->window->SetBrightness(brightness);

			if (!fullscreen && !fullscreenDesk) // Sliders won't work if we are on fullscreen mode
			{
				if (ImGui::SliderInt("Width", &width, 500, maxWidth)) // Width 
					App->window->SetSize(width, height); // Quizá se deberia cambiar el viewport de opengl para que no se deforme

				if (ImGui::SliderInt("Height", &height, 540, maxHeight)) // Height
					App->window->SetSize(width, height);
			}
			
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) // Fullscreen
			{
				App->window->SetFullscreen(fullscreen);
				fullscreenDesk = false;
			}

			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fullscreenDesk)) // Fullscreen desktop
			{
				App->window->SetFullscreenDesk(fullscreenDesk);
				fullscreen = false;
			}

			if (ImGui::Checkbox("Borderless", &borderless)) // Window is borderless
			{
				App->window->SetBorder(!borderless); // We send negated bool because SDL_SetWindowBordered takes true as putting borders
			}

			ImGui::SameLine();
			ImGui::Checkbox("Resizable", &resizable); // Window is resizable

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Restart to apply");
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
		}

		if (ImGui::CollapsingHeader("Input"))
		{
		}

		if (ImGui::CollapsingHeader("Audio"))
		{
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%d.%d.%d", App->ver.major, App->ver.minor, App->ver.patch);
			ImGui::PopStyleColor();

			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%d (Cache: %dkb)", App->cpuCount, App->cpuCacheSize);
			ImGui::PopStyleColor();

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%.1f Gb", App->ram);
			ImGui::PopStyleColor();

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::TextWrapped(App->caps.c_str());
			//ImGui::Text(App->caps.c_str());
			ImGui::PopStyleColor();

			ImGui::Separator();

			ImGui::Text("Integrated GPU: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text(App->gpuIntegratedModel.c_str());
			ImGui::PopStyleColor();

			ImGui::Text("Vendor: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text(App->gpuIntegratedVendor.c_str());
			ImGui::PopStyleColor();

			ImGui::Text("GPU: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("Undetectable with OpenGL2.0");
			ImGui::PopStyleColor();

			ImGui::Text("Vendor: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("Undetectable with OpenGL2.0");
			ImGui::PopStyleColor();

			ImGui::Text("Vram Budget: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%.1f Mb", App->VramTotal);
			ImGui::PopStyleColor();

			ImGui::Text("Vram Usage: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%.1f Mb", App->VramUsage);
			ImGui::PopStyleColor();

			ImGui::Text("Vram Available: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%.1f Mb", App->VramAvailable);
			ImGui::PopStyleColor();

			ImGui::Text("Vram Rreserved: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%.1f Mb", App->VramReserved);
			ImGui::PopStyleColor();
		}
		ImGui::End();
	}
	

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool Editor::CleanUp()
{
	delete about;
	about = nullptr;

	return true;
}