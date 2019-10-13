#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
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
	this->rotation_euler = euler;
	this->rotation_quat = Quat::FromEulerXYZ(euler.x * DEGTORAD, euler.y * DEGTORAD, euler.z * DEGTORAD);
	
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
	SetPosition(position);
	SetScale(scale);
	SetRotationQuat(rotation); // euler setting up too!
	
}

void ComponentTransform::InspectorInfo()
{
	if (ImGui::CollapsingHeader("Local Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Active Component", &active)) {

			if (ImGui::InputFloat3("Position", (float*)&position, 2))
				SetPosition(position);

			if (ImGui::InputFloat3("Rotation", (float*)&rotation_euler, 2))
				SetRotationEuler(rotation_euler);

			if (ImGui::InputFloat3("Scale", (float*)&scale, 2))
				SetScale(scale);
		}
	}
	// TODO: More info like bounding box..
}

