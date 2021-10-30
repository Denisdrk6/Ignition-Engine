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

	// Default checker texture
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

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
			const aiMesh* mesh = scene->mMeshes[i];

			// copy vertices
			ourMesh.num_vertex = mesh->mNumVertices;
			ourMesh.vertex = new float3[ourMesh.num_vertex]; // Each vertex has 3 coordinates
			memcpy(ourMesh.vertex, mesh->mVertices, sizeof(float) * ourMesh.num_vertex * 3);

			// copy faces
			if (mesh->HasFaces())
			{
				ourMesh.num_index = mesh->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_index * 2]; // assume each face is a triangle
				for (uint i = 0; i < mesh->mNumFaces; i++)
				{
					if (mesh->mFaces[i].mNumIndices != 3)
						App->log->AddLog("WARNING, geometry face with != 3 indices!\n");
					else
					{
						// Vertex indexes
						memcpy(&ourMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));

						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[0]);
						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[1]);
						ourMesh.indexes.push_back(mesh->mFaces[i].mIndices[2]);
					}
				}
			}

			// Default checker texture
			for (int i = 0; i < CHECKERS_HEIGHT; i++) {
				for (int j = 0; j < CHECKERS_WIDTH; j++) {
					int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
					ourMesh.checkerImage[i][j][0] = (GLubyte)c;
					ourMesh.checkerImage[i][j][1] = (GLubyte)c;
					ourMesh.checkerImage[i][j][2] = (GLubyte)c;
					ourMesh.checkerImage[i][j][3] = (GLubyte)255;
				}
			}


			// copy tex coords
			if (mesh->HasTextureCoords(0))
			{
				ourMesh.texCoords = new float2[mesh->mNumVertices]; // El array de texCoords tiene tamaño mNumVertices
				memcpy(ourMesh.texCoords, mesh->mTextureCoords[0], sizeof(float) * ourMesh.num_vertex * 2);
			}

			// copy normals
			for (uint i = 0; i < mesh->mNumVertices; ++i)
			{
				VertexData tempData;
				
				tempData.position.x = mesh->mVertices[i].x;
				tempData.position.y = mesh->mVertices[i].y;
				tempData.position.z = mesh->mVertices[i].z;

				if (mesh->HasNormals())
				{
					tempData.normals.x = mesh->mNormals[i].x;
					tempData.normals.y = mesh->mNormals[i].y;
					tempData.normals.z = mesh->mNormals[i].z;
				}

				if (mesh->HasTextureCoords(0))
				{
					tempData.texCoords.x = mesh->mTextureCoords[0][i].x;
					tempData.texCoords.y = mesh->mTextureCoords[0][i].y;
				}

				ourMesh.vertexData.push_back(tempData);
			}

			//LOAD FBX 1
			//glGenVertexArrays(1, (GLuint*)&ourMesh.id_vertexArray);
			//glBindVertexArray(ourMesh.id_vertexArray);


			//glEnableVertexAttribArray(0);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);
			//glEnableVertexAttribArray(1);
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float2), (void*)ourMesh.num_vertex);

			glGenBuffers(1, (GLuint*)&(ourMesh.id_vertex));
			glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_vertex * 3, ourMesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(ourMesh.id_index));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ourMesh.num_index, ourMesh.index, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(ourMesh.id_texCoords));
			glBindBuffer(GL_ARRAY_BUFFER, ourMesh.id_texCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh.num_vertex * 2, ourMesh.texCoords, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Establece como se guardan los pixeles
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			// Genera buffer de textura
			glGenTextures(1, &ourMesh.id_texture);
			// Decimos a OpenGL que trabajamos con este buffer de textura
			glBindTexture(GL_TEXTURE_2D, ourMesh.id_texture);
			// Parametros de la textura
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			// Establece los datos de la textura (como glBufferData)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
				0, GL_RGBA, GL_UNSIGNED_BYTE, ourMesh.checkerImage);
			// Unbind del buffer de la textura
			glBindTexture(GL_TEXTURE_2D, 0);

			meshes.push_back(ourMesh);
		}

		aiReleaseImport(scene);
	}
	else
		App->log->AddLog("Error loading scene %s\n", fileName);
}