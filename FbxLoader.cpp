#include "FbxLoader.h"
#include "Application.h"

// Contructr
FbxLoader::FbxLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
FbxLoader::~FbxLoader()
{
}

// Called before render is available
bool FbxLoader::Init()
{
	App->log->AddLog("Loading FBX loader\n");
	bool ret = true;

	// Activate assimp debugger
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool FbxLoader::Start()
{
	bool ret = true;


	return ret;
}

update_status FbxLoader::Update(float dt)
{

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool FbxLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void FbxLoader::LoadFbx(const char* fileName)
{
	MeshStorage ourMesh;
	//aiImportFileEx();
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			// copy vertices
			const aiMesh* mesh = scene->mMeshes[i];
			ourMesh.num_vertex = mesh->mNumVertices;
			ourMesh.vertex = new float3[ourMesh.num_vertex]; // Each vertex has 3 coordinates
			memcpy(ourMesh.vertex, mesh->mVertices, sizeof(float) * ourMesh.num_vertex * 3);

			// copy faces
			if (mesh->HasFaces())
			{
				ourMesh.num_index = mesh->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index]; // assume each face is a triangle
				for (uint i = 0; i < mesh->mNumFaces; ++i)
				{
					if (mesh->mFaces[i].mNumIndices != 3)
						App->log->AddLog("WARNING, geometry face with != 3 indices!\n");
					else
					{
						memcpy(&ourMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[0]);
						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[1]);
						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[2]);
					}
				}


			}

			// copy normals
			for (uint i = 0; i < mesh->mNumVertices; ++i)
			{
				VertexData tempData;

				float3 position;
				position.x = mesh->mVertices[i].x;
				position.y = mesh->mVertices[i].y;
				position.z = mesh->mVertices[i].z;
				tempData.position = position;

				if (mesh->HasNormals())
				{
					float3 normal;
					normal.x = mesh->mNormals[i].x;
					normal.y = mesh->mNormals[i].y;
					normal.z = mesh->mNormals[i].z;
					tempData.normals = normal;
				}

				if (mesh->HasTextureCoords(0))
				{
					float2 tempCoords;
					tempCoords.x = mesh->mTextureCoords[0][i].x;
					tempCoords.y = mesh->mTextureCoords[0][i].y;
					tempData.texCoords = tempCoords;
				}

				ourMesh.vertexData.push_back(tempData);
			}

			//LOAD FBX 1
			glGenBuffers(1, (GLuint*)&(ourMesh.id_vertex));
			glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_vertex * 3, ourMesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(ourMesh.id_index));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ourMesh.num_index, ourMesh.index, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			meshes.push_back(ourMesh);
		}

		aiReleaseImport(scene);
	}
	else
		App->log->AddLog("Error loading scene %s\n", fileName);
}