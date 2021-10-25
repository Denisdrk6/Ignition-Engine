#ifndef _FbxLoader_
#define _FbxLoader_

#include "Assimp/include/Assimp/cimport.h"
#include "Assimp/include/Assimp/scene.h"
#include "Assimp/include/Assimp/postprocess.h"
#include "Globals.h"
#pragma comment (lib, "Assimp/libx86/assimp-vc142-mt.lib")



// Stream log messages to Debug window
struct aiLogStream stream;
stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
aiAttachLogStream(&stream);

struct MeshStorage storage {
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};






#endif // _FbxLoader_
#pragma once
