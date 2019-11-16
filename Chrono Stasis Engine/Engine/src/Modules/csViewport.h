#pragma once
#include "csGlobals.h"

class ComponentCamera;

class Viewport 
{
public:
	Viewport();
	~Viewport();
	
	void PreUpdate(); 
	void Update();
	bool CleanUp();
	
public:

	ComponentCamera* camera = nullptr; 

	uint frameBuffer = 0;
	uint textureBuffer = 0;
	uint depthStencilBuffer = 0;

};