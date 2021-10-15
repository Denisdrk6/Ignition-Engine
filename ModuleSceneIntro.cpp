#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Math/float3.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	MYLOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Update: draw scene
update_status ModuleSceneIntro::Update(float dt)
{

    bool ret = true;

    PrimPlane p(0, 1, 0, 0);
    p.axis = true;
    p.Render();


    glBegin(GL_LINES);
    glLineWidth(2.0f);  //draw 1 Line
    glVertex3f(-10.f, -5.f, -3.f);
    glVertex3f(50.f, 50.f, 50.f);
    glEnd();


    //cube vertexs:
    float3 v0 = float3(0.5f, 0.5f, 0.5f);
    float3 v1 = float3(-0.5f, 0.5f, 0.5f);
    float3 v2 = float3(-0.5f, -0.5f, 0.5f);
    float3 v3 = float3(0.5f, -0.5f, 0.5f);
    float3 v4 = float3(0.5f, -0.5f, -0.5f);
    float3 v5 = float3(0.5f, 0.5f, -0.5f);
    float3 v6 = float3(-0.5f, 0.5f, -0.5f);
    float3 v7 = float3(-0.5f, -0.5f, -0.5f);

    glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

    // front face =================
    glVertex3fv(v0.ptr());
    glVertex3fv(v1.ptr());
    glVertex3fv(v2.ptr());

    glVertex3fv(v2.ptr());
    glVertex3fv(v3.ptr());
    glVertex3fv(v0.ptr());

    // right face =================
    glVertex3fv(v0.ptr());
    glVertex3fv(v3.ptr());
    glVertex3fv(v4.ptr());

    glVertex3fv(v4.ptr());
    glVertex3fv(v5.ptr());
    glVertex3fv(v0.ptr());

    // top face ===================
    glVertex3fv(v6.ptr());
    glVertex3fv(v1.ptr());
    glVertex3fv(v0.ptr());

    glVertex3fv(v0.ptr());
    glVertex3fv(v5.ptr());
    glVertex3fv(v6.ptr());

    // Back face =================
    //547, 765

    glVertex3fv(v5.ptr());
    glVertex3fv(v4.ptr());
    glVertex3fv(v7.ptr());

    glVertex3fv(v7.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v5.ptr());

    /*
    glVertex3fv(v5.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v4.ptr());

    glVertex3fv(v4.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v7.ptr());*/

    // Left face =================
    glVertex3fv(v1.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v7.ptr());

    glVertex3fv(v7.ptr());
    glVertex3fv(v2.ptr());
    glVertex3fv(v1.ptr());

    // Bottom face ===================
    //432, 274

    glVertex3fv(v4.ptr());
    glVertex3fv(v3.ptr());
    glVertex3fv(v2.ptr());

    glVertex3fv(v2.ptr());
    glVertex3fv(v7.ptr());
    glVertex3fv(v4.ptr());

    /*
    glVertex3fv(v2.ptr());
    glVertex3fv(v3.ptr());
    glVertex3fv(v7.ptr());

    glVertex3fv(v3.ptr());
    glVertex3fv(v4.ptr());
    glVertex3fv(v7.ptr());*/
    glEnd();

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
    MYLOG("Unloading Intro scene");

    return true;
}

