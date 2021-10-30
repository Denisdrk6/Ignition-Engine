#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

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

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

    /*glGenBuffers(1, (GLuint*)&(my_id));
    glBindBuffer(GL_ARRAY_BUFFER, my_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, (GLuint*)&(my_indices));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);*/

    //App->fbx->LoadFbx("Assets/warrior.fbx"); //Load an FBX file
   App->fbx->LoadFbx("Assets/cube.fbx");

	return ret;
}

// Update: draw scene
update_status ModuleSceneIntro::Update(float dt)
{

    bool ret = true;

    PrimPlane p(0, 1, 0, 0);
    p.axis = true;
    p.Render();

    //Wireframe mode

    //cube vertexs:
    /*float3 v0 = float3(0.5f, 0.5f, 0.5f);
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

    glVertex3fv(v5.ptr());
    glVertex3fv(v4.ptr());
    glVertex3fv(v7.ptr());

    glVertex3fv(v7.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v5.ptr());


    // Left face =================
    glVertex3fv(v1.ptr());
    glVertex3fv(v6.ptr());
    glVertex3fv(v7.ptr());

    glVertex3fv(v7.ptr());
    glVertex3fv(v2.ptr());
    glVertex3fv(v1.ptr());

    // Bottom face ===================

    glVertex3fv(v4.ptr());
    glVertex3fv(v3.ptr());
    glVertex3fv(v2.ptr());

    glVertex3fv(v2.ptr());
    glVertex3fv(v7.ptr());
    glVertex3fv(v4.ptr());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnd();*/

    //cube vertexs:
    /*float3 v0 = float3(0.5f, 0.5f, 0.5f);
    float3 v1 = float3(-0.5f, 0.5f, 0.5f);
    float3 v2 = float3(-0.5f, -0.5f, 0.5f);
    float3 v3 = float3(0.5f, -0.5f, 0.5f);
    float3 v4 = float3(0.5f, -0.5f, -0.5f);
    float3 v5 = float3(0.5f, 0.5f, -0.5f);
    float3 v6 = float3(-0.5f, 0.5f, -0.5f);
    float3 v7 = float3(-0.5f, -0.5f, -0.5f);

    glEnableClientState(GL_VERTEX_ARRAY);
    //Set
    uint my_id = 0;
    const int num_vertices = 6 * 6; // caras por vertices (cada cara tiene 2 triangulos, cada triangulo 3 vertices)
    float3 vertices[num_vertices] = { v0, v1, v2, v2, v3, v0, v0, v3, v4, v4, v5, v0, v6, v1, v0, v0, v5, v6, v5, v4, v7, v7, v6, v5, v1, v6, v7, v7, v2, v1, v4, v3, v2, v2, v7, v4};
    glGenBuffers(1, (GLuint*)&(my_id));
    glBindBuffer(GL_ARRAY_BUFFER, my_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

    //Draw
    glBindBuffer(GL_ARRAY_BUFFER, my_id);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glDisableClientState(GL_VERTEX_ARRAY);*/

    //glEnableClientState(GL_VERTEX_ARRAY);



    //////////////////

    //DRAW CUBE WITH VERTEX ARRAY & INDICES

    /*glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, my_id);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);*/

    //LOAD FBX 3
    for (int i = 0; !App->fbx->meshes.empty() && (i < App->fbx->meshes.size()); i++)
        App->renderer3D->DrawMesh(App->fbx->meshes.at(i));

    /*
    //__________buffers____________

    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

    //__________draw____________

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNET_INT, NULL);

    */

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
    App->log->AddLog("Unloading Intro scene\n");

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

