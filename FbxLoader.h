#ifndef __FbxLoader_H__
#define __FbxLoader_H__

#include "Module.h"
#include "Globals.h"

#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp-vc142-mt.lib")

struct MeshStorage {
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class FbxLoader : public Module
{
public:

	FbxLoader(Application* app, bool start_enabled = true);
	~FbxLoader();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();
	void LoadFbx(const char* filePath);

private:

	// Stream log messages to Debug window
	struct aiLogStream stream;
};


#endif // __FbxLoader_H__
#pragma once
