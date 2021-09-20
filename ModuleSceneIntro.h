#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "glut/glut.h"
#include "SString.h"

#define MAX_SNAKE 2
#define checkImageWidth 64
#define checkImageHeight 64
#define MAX_LIGHTS 8
#define TEX_SIZE 830.0


struct Image {

	unsigned long sizeX;

	unsigned long sizeY;

	char* data;

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void MapCreation();

	void display(float dt);

	void ResetScene();
};