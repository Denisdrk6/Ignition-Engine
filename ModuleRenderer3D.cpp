#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Math/float3.h"
//#include "SDL\include\SDL_opengl.h"

#include "ModuleCamera3D.h"
#include "FbxLoader.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->log->AddLog("Creating 3D Renderer context\n");
	bool ret = true;

	gl_context = SDL_GL_CreateContext(App->window->window);
	SDL_GL_MakeCurrent(App->window->window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // NECESARIO?

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	// NECESARIO?
	/*ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}*/

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context);
	ImGui_ImplOpenGL2_Init();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	
	//Create context
	/*context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		MYLOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}*/
	
	if(ret == true)
	{
		//Use Vsync
		if(vsync && SDL_GL_SetSwapInterval(1) < 0)
			App->log->AddLog("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->log->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->log->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->log->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		if(App->GLdepthTest) glEnable(GL_DEPTH_TEST);
		if (App->GLcullFace) glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		if (App->GLlightning) glEnable(GL_LIGHTING);
		if(App->GLcolorMaterial) glEnable(GL_COLOR_MATERIAL);
		if(App->GLtexture2D) glEnable(GL_TEXTURE_2D);
		if(App->GLlineSmooth) glEnable(GL_LINE_SMOOTH);
		if (App->GLfog)
		{
			glEnable(GL_FOG);
			float FogCol[3] = { App->fogR, App->fogG, App->fogB }; // Define a nice light grey
			glFogfv(GL_FOG_COLOR, FogCol); // Set the fog color
			if (App->fogLinear)glFogi(GL_FOG_MODE, GL_LINEAR); // GL_LINEAR constant is an integer.
			else
			{
				glFogi(GL_FOG_MODE, GL_EXP);
				glFogf(GL_FOG_DENSITY, App->fogDensity);
			}
			glFogf(GL_FOG_START, App->fogStart);
			glFogf(GL_FOG_END, App->fogEnd);
			gluPerspective(45.0f, 800.0f / 600.0f, 1.0f, 60.0f); // Fog allows us to shorten the far clipping plane
		}
	}

	glViewport(0, 0, App->window->width, App->window->height);
	
	// … check for errors
	GLenum err = glewInit();

	// Calculate projection matrix
	OnResize(App->window->width, App->window->height);

	if(!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// NECESARIO?
	/*if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	else SDL_GL_SwapWindow(App->window->window);*/

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->log->AddLog("Destroying 3D Renderer\n");

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::DrawMesh(MeshStorage mesh)
{
	bool ret = true;

	if (!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//LOAD FBX 2
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_index);
	
	//LOAD FBX 4
	glDrawElements(GL_TRIANGLES, mesh.num_index, GL_UNSIGNED_INT, NULL);

	//Draw normals
	if (drawNormals)
	{
		glBegin(GL_LINES);
		// Vertex normals
		/*for (int i = 0; i < mesh.vertexData.size(); i++)
		{
			float3 line = mesh.vertexData[i].normals + mesh.vertexData[i].position;
			glVertex3f(mesh.vertexData[i].position.x, mesh.vertexData[i].position.y, mesh.vertexData[i].position.z);
			glVertex3f(line.x, line.y, line.z);
		}*/

		// Face normals
		/*for (int i = 0; i <= mesh.vertexData.size() - 3; i += 3)
		{
			float3 centerPos;
			centerPos.x = (mesh.vertexData[i].position.x + mesh.vertexData[i + 1].position.x + mesh.vertexData[i + 2].position.x) / 3;
			centerPos.y = (mesh.vertexData[i].position.y + mesh.vertexData[i + 1].position.y + mesh.vertexData[i + 2].position.y) / 3;
			centerPos.z = (mesh.vertexData[i].position.z + mesh.vertexData[i + 1].position.z + mesh.vertexData[i + 2].position.z) / 3;

			float3 centerNormal;
			centerNormal.x = (mesh.vertexData[i].normals.x + mesh.vertexData[i + 1].normals.x + mesh.vertexData[i + 2].normals.x) / 3;
			centerNormal.y = (mesh.vertexData[i].normals.y + mesh.vertexData[i + 1].normals.y + mesh.vertexData[i + 2].normals.y) / 3;
			centerNormal.z = (mesh.vertexData[i].normals.z + mesh.vertexData[i + 1].normals.z + mesh.vertexData[i + 2].normals.z) / 3;


			float3 line = centerNormal + centerPos;
			glVertex3f(centerPos.x, centerPos.y, centerPos.z);
			glVertex3f(line.x, line.y, line.z);
		}*/

		for (int i = 0; i < mesh.indexes.size()-3; i+=3)
		{
			float3 centerPos;
			centerPos.x = (mesh.vertexData[mesh.indexes[i]].position.x + mesh.vertexData[mesh.indexes[i + 1]].position.x + mesh.vertexData[mesh.indexes[i + 2]].position.x) / 3;
			centerPos.y = (mesh.vertexData[mesh.indexes[i]].position.y + mesh.vertexData[mesh.indexes[i + 1]].position.y + mesh.vertexData[mesh.indexes[i + 2]].position.y) / 3;
			centerPos.z = (mesh.vertexData[mesh.indexes[i]].position.z + mesh.vertexData[mesh.indexes[i + 1]].position.z + mesh.vertexData[mesh.indexes[i + 2]].position.z) / 3;

			float3 centerNormal;
			centerNormal.x = (mesh.vertexData[mesh.indexes[i]].normals.x + mesh.vertexData[mesh.indexes[i + 1]].normals.x + mesh.vertexData[mesh.indexes[i + 2]].normals.x) / 3;
			centerNormal.y = (mesh.vertexData[mesh.indexes[i]].normals.y + mesh.vertexData[mesh.indexes[i + 1]].normals.y + mesh.vertexData[mesh.indexes[i + 2]].normals.y) / 3;
			centerNormal.z = (mesh.vertexData[mesh.indexes[i]].normals.z + mesh.vertexData[mesh.indexes[i + 1]].normals.z + mesh.vertexData[mesh.indexes[i + 2]].normals.z) / 3;
			centerNormal.Normalize();


			float3 line = centerNormal + centerPos;
			glVertex3f(centerPos.x, centerPos.y, centerPos.z);
			glVertex3f(line.x, line.y, line.z);
		}
		glEnd();
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	return ret;
}
