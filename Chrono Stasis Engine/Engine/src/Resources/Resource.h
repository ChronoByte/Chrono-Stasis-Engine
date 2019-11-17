#pragma once
#include "csGlobals.h"
#include <string>

class Resource
{
	friend class ModuleResources;
public:

	enum Type {

		R_UNKNOWN = 0,
		R_MESH,
		R_TEXTURE,
		R_SCENE
	};

	Resource(uint uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	uint GetUID() const;
	const char* GetFile() const;
	const char* GetImportedFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;
	
	virtual bool LoadInMemory() = 0;

protected:
	uint uid = 0;
	std::string file;
	std::string exported_file;
	std::string imported_file;

	Type type = Type::R_UNKNOWN;
	uint loaded = 0;
	
};
