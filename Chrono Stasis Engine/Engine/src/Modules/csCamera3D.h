#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "glm/glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void FocusAtObject();
	vec3 DistanceFromOrthonormalBasis();
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X = { 0.0f,0.0f,0.0f };
	vec3 Y = { 0.0f,0.0f,0.0f };
	vec3 Z = { 0.0f,0.0f,0.0f };
	vec3 Position = { 0.0f,0.0f,10.0f };
	vec3 Reference = { 0.0f,0.0f,0.0f };

	float cameraSpeed = 0;
	float mouseSensitivity = 0;
	float wheelSensitivity = 0;
	float zoomSpeed = 0;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool orbit = false;
};