#pragma once

#include "csGlobals.h"

class ComponentCamera; 

class Viewport
{
public: 

	Viewport(); 
	Viewport(const uint& width, const uint& height, ComponentCamera* camera = nullptr); 
	~Viewport(); 

	void SetCamera(ComponentCamera* camera); 
	void SetSize(const uint& width, const uint& height);

	void CreateBuffers();
	void SetView();

	void CreateTextures(); 

public: 
	ComponentCamera* camera = nullptr; 
	
	uint frameBuffer = 0; 
	uint depthBuffer = 0;

	uint renderTexture = 0;
	uint zBufferTexture = 0; 

	uint width = 0;
	uint height = 0; 
};