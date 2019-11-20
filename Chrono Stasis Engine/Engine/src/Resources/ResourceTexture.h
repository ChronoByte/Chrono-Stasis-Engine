#pragma once

#include "Resource.h"
#include "csGlobals.h"

class ResourceTexture : public Resource
{
public:
	enum Format {
		color_index,
		rgb,
		rgba,
		bgr,
		bgra,
		luminance,
		unknown
	};

	ResourceTexture(uint id, Resource::Type type);
	virtual ~ResourceTexture();
	

	virtual bool LoadInMemory() override;
	virtual bool UnloadOutMemory() override;
	
public:
	uint width = 0;
	uint height = 0;
	uint depth = 0;
	uint mips = 0;
	uint bytes = 0;
	uint gpu_id = 0;
	Format format = unknown;
};

