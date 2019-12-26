#include "csApp.h"
#include "csTextureLoader.h"

#include "DevIL\include\ilut.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"

#include "gl/GL.h"
#include "csFileSystem.h"

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
	LOG(" --- Creating Texture Importerer ---");
	LOG("Initializing DevIL library.");
	bool ret = true;
	ILuint devilError = 0;

	LOG("Initializing IL");
	ilInit();
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing IL. Error: %s", iluErrorString(devilError));
		ret = false;
	}

	LOG("Initializing ILU");
	iluInit();
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing ILU. Error: %s", iluErrorString(devilError));
		ret = false;
	}

	LOG("Initializing ILUT");
	ilutRenderer(ILUT_OPENGL);
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("TEXTURE LOADER: Error initializing ILUT. Error: %s", iluErrorString(devilError));
		ret = false;
	}
	else
	{
		LOG("DevIL Library Initialized successfully.");
		LOG("DevIL Version v%i.%i.%i", 1, 8, 0);
	}


	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT); // Setting as default origin to load images in upper left mode
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
	testTexture->path = "Checkers Texture"; 

	CreateTextureBuffers(testTexture); 
}

TextureInfo * ModuleTextureLoader::GetCheckersTexture() const
{
	return testTexture;
}

TextureInfo* ModuleTextureLoader::LoadTexture(const char* tex_file)
{
	TextureInfo* t = nullptr; 
	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	LOG("Loading texture at: %s", tex_file); 
	if ((bool)ilLoadImage(tex_file))
	{
		//Checking image origin
		ILinfo img_info;
		iluGetImageInfo(&img_info);

		if (img_info.Origin != IL_ORIGIN_LOWER_LEFT) 
			iluFlipImage();

		if (ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
			// If its converted properly, create the data
			t = new TextureInfo;

			StorageTextureData(t);
			CreateTextureBuffers(t);
			t->path = tex_file; 

			textures.push_back(t);
		}
		else
			LOG("Image could not be converted, error: %s", iluErrorString(ilGetError()));		
	}
	else 
		LOG("Error loading texture at %s, error: %s", tex_file, iluErrorString(ilGetError()));
	

	ilDeleteImages(1, &imageID);
	return t;
}

// Finds texture in our vector of loaded textures
TextureInfo * ModuleTextureLoader::FindLoadedTextureWithPath(const char * path) const
{
	for (uint i = 0; i < textures.size(); ++i)
	{
		if (strcmp(path, textures[i]->path.c_str()) == 0)
		{
			return textures[i]; 
		}
	}

	return nullptr;
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

TextureInfo* ModuleTextureLoader::LoadTextureIcon(const char* path)
{
	TextureInfo* icon = nullptr;
	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	LOG("Loading texture at: %s", path);
	if ((bool)ilLoadImage(path))
	{
		//Checking image origin
		ILinfo img_info;
		iluGetImageInfo(&img_info);

		//if (img_info.Origin != IL_ORIGIN_LOWER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
			// If its converted properly, create the data
			icon = new TextureInfo;

			StorageTextureData(icon);
			CreateTextureBuffers(icon);
			icon->path = path;

			icons.push_back(icon);
		}
		else
			LOG("Image could not be converted, error: %s", iluErrorString(ilGetError()));
	}
	else
		LOG("Error loading texture at %s, error: %s", path, iluErrorString(ilGetError()));


	ilDeleteImages(1, &imageID);
	return icon;

}

bool ModuleTextureLoader::Import(const char* path, std::string& library_file, UID& uuid)
{
	bool ret = false;

	uint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	char* buffer = nullptr;
	uint size = App->fs->ReadFile(path, &buffer); //fill buffer from texture file data

	if(ilLoadL(IL_TYPE_UNKNOWN, buffer, size)) //Load image data from Lumps (Memory)
	{
		LOG("Texture file [%s] loading from memory successfully", path);
		ret = true;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Set compression format (DXT5)

		ILuint size;
		size = ilSaveL(IL_DDS, NULL, 0); // Get size from DDS buffer

		if (size > 0) 
		{
			ILubyte* data;
			data = new ILubyte[size];

			if (ilSaveL(IL_DDS, data, size) > 0) // Saving image data to DDS buffer
			{
				//Generate UUID 
				if (uuid == 0u)
					uuid = GenerateUUID();

				//Create Own format Texture file
				std::string output_file(L_TEXTURES_FOLDER + std::to_string(uuid) + TEXTURES_EXTENSION);
				App->fs->WriteFile(output_file.c_str(), (char*)data, size);
				library_file = output_file; 
				LOG("Own format file exported successfully at [%s]", library_file);
			}
			RELEASE_ARRAY(data);
		}
	}
	else {
		LOG("Texture file [&s] loading from memory failed - IL error: %s", path, iluErrorString(ilGetError()));
		ret = false;
	}

	ilDeleteImages(1, &imageID);
	RELEASE_ARRAY(buffer);

	return ret;
}


