#include "ComponentTransform.h"


ComponentTransform::ComponentTransform() : Component(nullptr)
{
	type = ComponentType::C_TRANSFORM; 
}

ComponentTransform::~ComponentTransform()
{
}


const float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_matrix;
}

const void ComponentTransform::SetLocalTransform(const float4x4& local)
{
	this->local_matrix = local;
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_matrix;
}

const void ComponentTransform::SetPosition(const float3& pos)
{
	this->position = pos;

}

const void ComponentTransform::SetRotationEuler(const float3& euler)
{
	float3 diff = euler - rotation_euler;
	Quat quat_diff = Quat::FromEulerXYZ(diff.x * DEGTORAD, diff.y * DEGTORAD, diff.z * DEGTORAD);
	this->rotation_quat = rotation_quat * quat_diff;
	this->rotation_euler = euler;
}

const void ComponentTransform::SetRotationQuat(const Quat& quat)
{
	this->rotation_quat = quat;
	this->rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
}

const void ComponentTransform::SetScale(const float3& scale)
{
	this->scale = scale;
}

const float3 ComponentTransform::GetPosition() const
{
	return position;
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return rotation_euler;
}

const Quat ComponentTransform::GetRotationQuat() const
{
	return rotation_quat;
}

const float3 ComponentTransform::GetScale() const
{
	return scale;
}

void ComponentTransform::SetupTransform(const float3& position, const float3& scale, const Quat& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation_quat = rotation;

}
