#include "Resource.h"

Resource::Resource(uint uid, Resource::Type type)
{
}

Resource::~Resource()
{
}

Resource::Type Resource::GetType() const
{
	return type;
}

UID Resource::GetUID() const
{
	return uid;
}

const char* Resource::GetFile() const
{
	return file.c_str();
}

const char* Resource::GetName() const
{
	return name.c_str();
}

const char* Resource::GetExportedFile() const
{
	return exported_file.c_str();
}

bool Resource::IsLoadedToMemory() const
{
	return (loaded > 0);
}

bool Resource::LoadToMemory()
{
	bool ret = false;

	if (loaded == 0)
	{
		LOG("Resource loaded successfully");
		ret = LoadInMemory();
	}

	loaded++;

	return ret;
}

bool Resource::UnloadFromMemory()
{
	if (loaded > 0)
		loaded--;
	if (loaded == 0)
	{
		LOG("Resource Unloaded successfully");
		UnloadOutMemory();
	}

	return false;
}

uint Resource::CountReferences() const
{
	return loaded;
}
