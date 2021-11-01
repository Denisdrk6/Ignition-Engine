#ifndef __FbxLoader_H__
#define __FbxLoader_H__

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "Math/float4.h"
#include "Math/float3.h"
#include "Math/float2.h"

#include "glew.h"

#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp-vc142-mt.lib")

class GameObject;

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

	uint id_texCoords = 0;
	float2* texCoords = nullptr;

	uint id_texture = 0;
	//GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	std::vector<VertexData> vertexData;
	std::vector<uint> indexes;

	uint vao;
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

	GLuint LoadImageTexture(const char* filePath);
	void RecursiveLoad(aiNode* node, GameObject* parent, const aiScene* scene);

private:

	// Stream log messages to Debug window
	struct aiLogStream stream;

	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
};

#endif // __FbxLoader_H__
#pragma once