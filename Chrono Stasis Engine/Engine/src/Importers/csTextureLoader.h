#pragma once

#include "csModule.h"
#include "csGlobals.h"

struct TextureInfo
{
	GLubyte* image = nullptr;
	GLuint id = 0;
	int width = 0;
	int height = 0;
	std::string path;
};

class ModuleTextureLoader : public Module
{
public:

	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	
	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

public:
	void LoadCheckeredTexture();
	TextureInfo* GetCheckersTexture() const; 
	TextureInfo* LoadTexture(const char* tex_file);
	void CreateTextureBuffers(TextureInfo* tex);
	void StorageTextureData(TextureInfo* tex);
	void DeleteTextures();

public:
	TextureInfo* testTexture = nullptr;
	std::vector<TextureInfo*> textures;
};
