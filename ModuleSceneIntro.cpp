#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "imgui/imgui.h"

#include "FbxLoader.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
    App->log->AddLog("Loading Intro Scene\n");
	bool ret = true;
    App->fbx->LoadFbx("Assets/BakerHouse.fbx");

   //game_objects.push_back(CreateGameObject());

	return ret;
}

// Update: draw scene
update_status ModuleSceneIntro::Update(float dt)
{

    bool ret = true;

    PrimPlane p(0, 1, 0, 0);
    p.axis = true;
    p.Render();

    

    //LOAD FBX 3
    for (int i = 0; !App->fbx->meshes.empty() && (i < App->fbx->meshes.size()); i++)
        App->renderer3D->DrawMesh(App->fbx->meshes.at(i));

    for (int i = 0; i < game_objects.size(); i++)
        game_objects.at(i)->Update();

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
    App->log->AddLog("Unloading Intro scene\n");

    for (int i = 0; i < game_objects.size(); i++)
    {
        game_objects.at(i)->CleanUp();
        RELEASE(game_objects.at(i));
    }

    game_objects.clear();

    /*
    //__________Unbind buffers____________

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
    glBindBuffer(GL_TEXTURE_2D, 0);

    //___________DISABLE STATES__________

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    */

    return true;
}

GameObject* ModuleSceneIntro::CreateGameObject()
{
    GameObject* gameObject = new GameObject();

    return gameObject;
}

void ModuleSceneIntro::DrawHierarchy()
{

    ImGui::SetNextWindowSize({ 300.0f, (float)App->window->height - 220.0f });
    ImGui::SetNextWindowPos({ 0.0f, 20.0f }); // Main menu bar is 20px high

    ImGui::Begin("Hierarchy", (bool*)true, ImGuiWindowFlags_NoCollapse); // Cannot be manually closed by user

    for (int i = 0; i < game_objects.size(); i++)
    {
        if (game_objects[i]->children.size() > 0)
        {
            if (ImGui::TreeNodeEx(game_objects[i]->name.c_str()))
            {
                for (int j = 0; j < game_objects[i]->children.size(); j++)
                {
                    DrawRecursive(game_objects[i]->children[j]);
                }
                ImGui::TreePop();
            }
        }

        else
        {
            ImGui::Text(game_objects[i]->name.c_str());
        }
    }

    //if (ImGui::IsItemClicked())

    ImGui::End();
}

void ModuleSceneIntro::DrawRecursive(GameObject* parent)
{
    if (parent->children.size() > 0)
    {
        if (ImGui::TreeNodeEx(parent->name.c_str()))
        {
            for (int j = 0; j < parent->children.size(); j++)
            {
                DrawRecursive(parent->children[j]);
            }
            ImGui::TreePop();
        }
    }

    else
    {
        ImGui::Text(parent->name.c_str());
    }
}
