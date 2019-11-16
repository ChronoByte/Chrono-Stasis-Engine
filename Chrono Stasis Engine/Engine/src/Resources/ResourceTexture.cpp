#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(uint id, Resource::Type type):Resource(id, type)
{

}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	return false;
}
