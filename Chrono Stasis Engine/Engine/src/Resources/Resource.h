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

	Resource(UID uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetName() const;
	//const char* GetImportedFile() const;
	const char* GetExportedFile() const;

	void SetName(const char* name);
	void SetFile(const char* file);
	void SetExportedFile(const char* exported_file);

	uint CountReferences() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	bool UnloadFromMemory();

	virtual bool LoadInMemory() = 0;
	virtual bool UnloadOutMemory() = 0;

protected:
	UID uid = 0;
	std::string file;
	std::string name;
	std::string exported_file;
	std::string imported_file;

	Type type = Type::R_UNKNOWN;
	uint loaded = 0;
	
};
