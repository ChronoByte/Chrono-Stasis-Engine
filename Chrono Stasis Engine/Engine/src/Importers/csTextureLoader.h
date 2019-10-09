#pragma once

#include "csModule.h"
#include "csGlobals.h"

struct TextureInfo
{
	GLuint id = 0;
	GLubyte image = 0;
	int width = 0;
	int height = 0;
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
	GLubyte LoadCheckeredTexture();
	bool LoadTexture(const char* tex_file);
	void CreateTextureBuffers(TextureInfo* tex);
public:
	TextureInfo* t = nullptr;
	std::vector<TextureInfo*> textures;
};
