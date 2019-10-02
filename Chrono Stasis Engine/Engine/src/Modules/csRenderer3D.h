#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "glm/glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
public:
	void SetDepthTest(const bool& depth) const;
	void SetCullFace(const bool& cullFace) const;
	void SetLighting(const bool& light) const;
	void SetSmoothPolygon(const bool& pSmooth) const;
	void SetSmoothLine(const bool& lSmooth) const;
	void SetTexture2D(const bool& tex2D) const;
	void SetColorMaterial(const bool& colorMaterial) const;
	void SetWireframe(const bool& cullFace) const;

	void ModuleRenderer3D::SetLightAmbient();
	//TODO: Set more gl graphic funcs
	
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float3 a_light = float3(1.0f,1.0f,1.0f);
	
};