#pragma once

#include "csComponent.h"
#include "csGlobals.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"

#include "SDL/include/SDL_rect.h"

struct BoundingBox {

	AABB aabb;
	OBB obb;

};

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	void Update(float dt) override;
	void OnDebugDraw() override; 

	void DrawBoundingBox();
	const float4x4 GetLocalTransform() const;
	const float4x4 GetGlobalTransform() const;
	const void SetLocalTransform(const float4x4& local);

	void CalculateTransformRecursively();
	void UpdateBoundingBox();
	void UpdateParentBoundingBox();
	void ForceParentBoundingBox();

	const void SetPosition(const float3& pos);
	const void SetRotationEuler(const float3& euler);
	const void SetRotationQuat(const Quat& quat);
	const void SetScale(const float3& scale);

	const float3 GetPosition() const;
	const float3 GetRotationEuler() const;
	const Quat GetRotationQuat() const;
	const float3 GetScale() const;
	
	const float3 GetGlobalPosition() const; 
	const float3 GetGlobalRotationEuler() const;
	const Quat GetGlobalRotationQuat() const;
	const float3 GetGlobalScale() const;

	void SetupTransform(const float3& position, const float3& scale, const Quat& rotation);

	const void SetBoundingBox(const AABB& bb);
	const AABB GetBoundingBox() const;
	SDL_Rect GetBoundingBox2D() const; 

	void InspectorInfo();

	// Parser
	void Save(RJSON_Value* component) const;
	void Load(RJSON_Value* component);

	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

public:

	float4x4 local_matrix = float4x4::identity;
	float4x4 global_matrix = float4x4::identity;

	BoundingBox boundingBox;
	bool drawBoundingBox = false;

	bool toRecalculateTransform = false;

private: 

	float3   position = float3::zero;
	float3   rotation_euler = float3::zero;
	Quat     rotation_quat = Quat::identity;
	float3   scale = float3(1.f,1.f,1.f);
	
	float3   globalPosition = float3::zero;
	float3   globalRotation_euler = float3::zero;
	Quat     globalRotation_quat = Quat::identity;
	float3   globalScale = float3(1.f,1.f,1.f);
};