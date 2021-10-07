#include "Globals.h"
#include "Application.h"
#include "Editor.h"

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//about = new Tab(true);
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
	App->window->maxWidth = DM.w;
	App->window->maxHeight = DM.h;

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

			if (ImGui::TreeNode("About"))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f , 0.96f , 0.33f , 0.05f });

				ImGui::Text("");

				ImGui::Text("Ignition Engine V0.1");
				ImGui::Text("The next-gen 3D game engine.");
				ImGui::Text("By");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("Denis Deconinck")) App->RequestBrowser("https://github.com/Denisdrk6");
				ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text("&");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("Pol Pallares")) App->RequestBrowser("https://github.com/Zeta115");
				ImGui::PopStyleColor();

				ImGui::Separator();

				ImGui::Text("3rd Party Libraries Used:");

				ImGui::BulletText("SDL2");
				ImGui::SameLine();
				ImGui::Text("%d.%d.%d", App->SDLversion.major, App->SDLversion.minor, App->SDLversion.patch);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("https://www.libsdl.org/download-2.0.php");
				ImGui::PopStyleColor();

				ImGui::BulletText("OpenGL");
				ImGui::SameLine();
				ImGui::Text("%d.%d", App->GLverMajor, App->GLverMinor);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("https://www.opengl.org//");
				ImGui::PopStyleColor();

				ImGui::BulletText("Glew 2.0.0");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("http://glew.sourceforge.net/");
				ImGui::PopStyleColor();

				ImGui::BulletText("ImGui");
				ImGui::SameLine();
				ImGui::Text(ImGui::GetVersion());
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("https://github.com/ocornut/imgu");
				ImGui::PopStyleColor();

				ImGui::BulletText("MathGeoLib 1.5");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("https://github.com/juj/MathGeoLib");
				ImGui::PopStyleColor();

				ImGui::BulletText("PugiXML 1.6");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.33f, 0.8f });
				if (ImGui::Button("WEB")) App->RequestBrowser("https://github.com/zeux/pugixml");
				ImGui::PopStyleColor();

				ImGui::Separator();

				ImGui::Text("MIT License");
				ImGui::Text("");

				ImGui::Text("Copyright(c) 2021 Denisdrk6");
				ImGui::Text("");

				ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
				ImGui::Text("of this softwareand associated documentation files(the \"Software\"), to deal");
				ImGui::Text("in the Software without restriction, including without limitation the rights");
				ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
				ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
				ImGui::Text("furnished to do so, subject to the following conditions :");
				ImGui::Text("");

				ImGui::Text("The above copyright noticeand this permission notice shall be included in all");
				ImGui::Text("copies or substantial portions of the Software.");
				ImGui::Text("");

				ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
				ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
				ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
				ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
				ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
				ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
				ImGui::Text("SOFTWARE.");

				ImGui::PopStyleColor();
				ImGui::TreePop();
			}

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

		App->log->Draw("LOG", NULL, App->window);
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
		ImGui::SetNextWindowSize({ 300.0f, (float)App->window->height - 20.0f });
		ImGui::SetNextWindowPos({ 0.0f, 20.0f }); // Main menu bar is 20px high

		ImGui::Begin("Configuration", &config, ImGuiWindowFlags_AlwaysAutoResize);
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
			ImGui::Text("%d", App->maxFPS);
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
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.1f, 1.0f)) // Brightness
				App->window->SetBrightness(App->window->brightness);

			if (!App->window->fullscreen && !App->window->fullscreenDesk) // Sliders won't work if we are on fullscreen mode
			{
				if (ImGui::SliderInt("Width", &App->window->width, 500, App->window->maxWidth)) // Width 
					App->window->SetSize(App->window->width, App->window->height); // Quizá se deberia cambiar el viewport de opengl para que no se deforme

				if (ImGui::SliderInt("Height", &App->window->height, 540, App->window->maxHeight)) // Height
					App->window->SetSize(App->window->width, App->window->height);
			}
			
			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen)) // Fullscreen
			{
				App->window->SetFullscreen(App->window->fullscreen);
				App->window->fullscreenDesk = false;
				SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);
			}

			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &App->window->fullscreenDesk)) // Fullscreen desktop
			{
				App->window->SetFullscreenDesk(App->window->fullscreenDesk);
				App->window->fullscreen = false;
				SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);
			}

			if (ImGui::Checkbox("Borderless", &App->window->borderless)) // Window is borderless
			{
				App->window->SetBorder(!App->window->borderless); // We send negated bool because SDL_SetWindowBordered takes true as putting borders
			}

			ImGui::SameLine();
			ImGui::Checkbox("Resizable", &App->window->resizable); // Window is resizable

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Restart to apply");
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			ImGui::Checkbox("Vsync", &App->renderer3D->vsync);

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Restart to apply");
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse position: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("(%d, %d)", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::PopStyleColor();

			ImGui::Text("Mouse wheel motion: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text("%d", App->input->GetMouseZ());
			ImGui::PopStyleColor();
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
			ImGui::Text("%d.%d.%d", App->SDLversion.major, App->SDLversion.minor, App->SDLversion.patch);
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

			ImGui::Text("GPU: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text(App->gpuIntegratedModel.c_str());
			ImGui::PopStyleColor();

			ImGui::Text("Vendor: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.15f, 0.96f, 0.69f, 0.8f });
			ImGui::Text(App->gpuIntegratedVendor.c_str());
			ImGui::PopStyleColor();
		}
		ImGui::End();
	}
	

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool Editor::CleanUp()
{
	/*delete about;
	about = nullptr;*/

	return true;
}