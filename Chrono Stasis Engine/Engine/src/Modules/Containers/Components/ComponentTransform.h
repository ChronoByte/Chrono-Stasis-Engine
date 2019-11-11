#pragma once

#include "csComponent.h"
#include "csGlobals.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(GameObject* parent);
	virtual ~ComponentTransform();

	void Update(float dt) override;
	void DrawBoundingBox();
	const float4x4 GetLocalTransform() const;
	const void SetLocalTransform(const float4x4& local);
	const float4x4 GetGlobalTransform() const;
	void RecursiveCalcTransform(GameObject* parent);

	const void SetPosition(const float3& pos);
	const void SetRotationEuler(const float3& euler);
	const void SetRotationQuat(const Quat& quat);
	const void SetScale(const float3& scale);

	const float3 GetPosition() const;
	const float3 GetRotationEuler() const;
	const Quat GetRotationQuat() const;
	const float3 GetScale() const;

	void SetupTransform(const float3& position, const float3& scale, const Quat& rotation);

	const void SetBoundingBox(const AABB& bb);
	const AABB GetBoundingBox() const;

	void InspectorInfo();

public:

	float4x4 local_matrix = float4x4::identity;
	float4x4 global_matrix = float4x4::identity;

	float3   position = float3::zero;
	float3   rotation_euler = float3::zero;
	Quat     rotation_quat = Quat::identity;
	float3   scale = float3::zero;

	AABB bounding_box;
	bool drawBoundingBox = false;

	bool toRecalculateTransform = false; 
};