#include "csApp.h"
#include "ResourceTexture.h"
#include "csFileSystem.h"

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

ResourceTexture::ResourceTexture(uint id, Resource::Type type):Resource(id, type)
{

}

ResourceTexture::~ResourceTexture()
{
	UnloadOutMemory();
}


// ------------ LOAD / UNLOAD BUFFERS ------------ //
bool ResourceTexture::LoadTextureBuffer()
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fs->ReadFile(exported_file.c_str(), &buffer);

	if (buffer != nullptr && size > 0)
	{
		ILuint ilImage;
		ilGenImages(1, &ilImage);
		ilBindImage(ilImage);

		if (ilLoadL(IL_DDS, (const void*)buffer, size));
		{
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);

			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();

			glGenTextures(1, &gpu_id);
			glBindTexture(GL_TEXTURE_2D, gpu_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageInfo.Width, ImageInfo.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			width = ImageInfo.Width;
			height = ImageInfo.Height;

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		ilDeleteImages(1, &ilImage);
		RELEASE_ARRAY(buffer);
		ret = true;
	}
	else
	{
		LOG("Error loading texture: %s", exported_file.c_str());
		ret = false;
	}

	return ret;
}

bool ResourceTexture::UnloadTextureBuffer()
{
	glDeleteTextures(1, &gpu_id);
	return true;
}

// ------------  LOAD / UNLOAD COUNTING REFERENCE ------------ //
bool ResourceTexture::LoadInMemory()
{
	bool ret = LoadTextureBuffer();
	return ret;
}

bool ResourceTexture::UnloadOutMemory()
{
	bool ret = UnloadTextureBuffer();
	return ret;
}
