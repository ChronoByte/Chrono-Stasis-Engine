#pragma once

#include "csComponent.h"
#include "csGlobals.h"
#include "Color.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"


class ComponentCamera : public Component
{
public:

	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	void Update(float dt) override;
	void OnDebugDraw() override; 

	void UpdateTransform();
	
	void DrawFrustum(); 
	void InspectorInfo();

	bool CheckAABBInsideFrustum(const OBB& myAbb);
	void SetInitially();
	void UpdateRatio(bool axisVertical, float verticalFOV, float horizontalFOV = 0.f);

	void LookAt(const float3& position);
	// --------------------------------------------
	// ------------- Sets ------------------

	// Pos
	void SetPos(const float3& pos); 
	// Vectors
	void SetFrontVector(const float3& front); 
	void SetUpVector(const float3& up); 
	// Planes
	void SetFarPlaneDistance(const float& distance); 
	void SetNearPlaneDistance(const float& distance); 
	// FoV
	void SetHorizontalFOV(const float& fov); 
	void SetVerticalFOV(const float& fov); 
	// Color
	void SetColor(const Color& set); 
	// ------------- Gets ------------------
	// Pos
	float3 GetPos() const; 
	// Vectors
	float3 GetFrontVector() const;
	float3 GetUpVector() const;
	// Planes
	float GetFarPlaneDistance() const;
	float GetNearPlaneDistance() const;
	// FoV
	float GetHorizontalFOV() const;
	float GetVerticalFOV() const;
	// Matrix
	float4x4 GetViewMatrix() const; 
	float4x4 GetProjectionMatrix() const; 
	// Culling
	bool isCulling() const; 
	// Color
	const float* GetColor() const; 
	// --------------------------------------------



	// Parser
	void Save(RJSON_Value* component) const;
	void Load(RJSON_Value* component);

public: 

	bool isMainCamera = false; 
	Frustum frustum;

private:

	float bgColor[4] = { 0.6784f, 0.8392f, 1.f, 1.f };
	float aspectRatio = 0.0f;
	bool culling = true; 
};