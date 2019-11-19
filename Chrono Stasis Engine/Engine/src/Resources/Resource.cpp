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
