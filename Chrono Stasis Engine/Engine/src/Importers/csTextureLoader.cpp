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

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT); // Setting as default origin to load images in upper left mode
	return ret;
}

bool ModuleTextureLoader::Start()
{
	LoadCheckeredTexture();

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
	DeleteTextures();

	delete testTexture;
	testTexture = nullptr; 

	return true;
}

void ModuleTextureLoader::LoadCheckeredTexture()
{
	testTexture = new TextureInfo; 

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
	testTexture->image = &checkImage[0][0][0];
	testTexture->width = checker_width; 
	testTexture->height = checker_height;

	CreateTextureBuffers(testTexture); 
}

TextureInfo* ModuleTextureLoader::LoadTexture(const char* tex_file)
{
	TextureInfo* t = nullptr; 
	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if ((bool)ilLoadImage(tex_file))
	{
		//Checking image origin
		ILinfo img_info;
		iluGetImageInfo(&img_info);

		if (img_info.Origin != IL_ORIGIN_UPPER_LEFT) 
			iluFlipImage();

		t = new TextureInfo;

		if (ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
			StorageTextureData(t);
			CreateTextureBuffers(t);
		}
		else
			LOG("Image could not be converted, error: %s", iluErrorString(ilGetError()));
		
		textures.push_back(t);
	}
	else 
		LOG("Error loading texture %s", iluErrorString(ilGetError()));
	

	ilDeleteImages(1, &imageID);
	return t;
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

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), tex->width, tex->height,
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, tex->image);
}

void ModuleTextureLoader::StorageTextureData(TextureInfo* tex)
{
	tex->image = ilGetData();
	tex->width = ilGetInteger(IL_IMAGE_WIDTH);
	tex->height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	//TODO: fill with more texture data

}

void ModuleTextureLoader::DeleteTextures()
{
	for (int i = 0; i < textures.size(); ++i)
	{
		delete textures[i];
	}
	textures.clear();
}


