#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "glm/glmath.h"

class ComponentCamera; 

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const float3& Spot);
	void Move(const vec3 &Movement);
	void FocusAtObject();
	float3 DistanceFromOrthonormalBasis();
	float4x4 GetViewMatrix();

public:
	
	float3 reference = float3::zero; 

	float cameraSpeed = 0;
	float mouseSensitivity = 0;
	float wheelSensitivity = 0;
	float zoomSpeed = 0;

	bool cameraControls = false;

	ComponentCamera* fakeCamera = nullptr; 

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool orbit = false;
};