#include "csApp.h"
#include "csTextureLoader.h"

#include "DevIL\include\ilut.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"

#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/DevIL.lib") 
#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/ILUT.lib")

ModuleTextureLoader::ModuleTextureLoader(bool start_enabled)
{
}

ModuleTextureLoader::~ModuleTextureLoader()
{
}

bool ModuleTextureLoader::Init(JSON_Object* node)
{
	bool ret = false;
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("TEXTURE LOADER: DevIL IL, ILU , ILUT not same version!");
		
	}
	else {
		ilInit();
		LOG("TEXTURE LOADER: Initializating DevIL...");
		ret = true;
	}
	
	return ret;
}

bool ModuleTextureLoader::Start()
{
	return true;
}

update_status ModuleTextureLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextureLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextureLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleTextureLoader::CleanUp()
{
	return true;
}

GLubyte ModuleTextureLoader::LoadCheckeredTexture()
{
	const int checker_height = 120;
	const int checker_width = 120;
	GLubyte checkImage[checker_height][checker_width][4];
	for (int i = 0; i < checker_height; i++) {
		for (int j = 0; j < checker_width; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	return checkImage[checker_height][checker_width][4];
}
