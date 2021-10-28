#ifndef __FbxLoader_H__
#define __FbxLoader_H__

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "Math/float4.h"
#include "Math/float3.h"
#include "Math/float2.h"

#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp-vc142-mt.lib")

struct VertexData {
	float4 color;
	float3 position;
	float3 normals;
	float2 texCoords;
};

struct MeshStorage {
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float3* vertex = nullptr;
	std::vector<VertexData> vertexData;
};

class FbxLoader : public Module
{
public:

	FbxLoader(Application* app, bool start_enabled = true);
	~FbxLoader();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void LoadFbx(const char* filePath);
	std::vector< MeshStorage > meshes;

private:

	// Stream log messages to Debug window
	struct aiLogStream stream;
};

#endif // __FbxLoader_H__
#pragma once