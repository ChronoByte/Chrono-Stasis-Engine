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
	name = "Texture Loader";
}

ModuleTextureLoader::~ModuleTextureLoader()
{
}

bool ModuleTextureLoader::Init(JSON_Object* node)
{
	bool ret = true;
	ILuint devilError = 0;

	ilInit();
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing IL. Error: %s", iluErrorString(devilError));
		ret = false;
	}

	iluInit();
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing ILU. Error: %s", iluErrorString(devilError));
		ret = false;
	}

	ilutRenderer(ILUT_OPENGL);
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing ILUT. Error: %s", iluErrorString(devilError));
		ret = false;
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

bool ModuleTextureLoader::LoadTexture(const char* tex_file)
{
	
	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if ((bool)ilLoadImage(tex_file))
	{
		t = new TextureInfo;

		if (ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
			CreateTextureBuffers(t);
		else
			LOG("Image could not be converted, error: %s", iluErrorString(ilGetError()));
		
		textures.push_back(t);
	}
	else 
		LOG("Error loading texture %s", iluErrorString(ilGetError()));
	

	ilDeleteImages(1, &imageID);
	return false;
}

void ModuleTextureLoader::CreateTextureBuffers(TextureInfo* tex)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &tex->id);
	glBindTexture(GL_TEXTURE_2D, tex->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
}


