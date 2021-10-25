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

update_status FbxLoader::PreUpdate(float dt)
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
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
		App->log->AddLog("Error loading scene % s", fileName);
}