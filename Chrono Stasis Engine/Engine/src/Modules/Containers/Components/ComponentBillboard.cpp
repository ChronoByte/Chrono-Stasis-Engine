#include "ComponentBillboard.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "csApp.h"
#include "csCamera3D.h"

ComponentBillboard::ComponentBillboard(GameObject* parent) : Component(parent)
{
	UUID = GenerateUUID();
	type = ComponentType::C_BILLBOARD;
	name = "Billboard"; 
}

ComponentBillboard::~ComponentBillboard()
{
}

void ComponentBillboard::Update(float dt)
{

	// For the moment, debug with editor camera:

	ComponentCamera* camera = App->camera->fakeCamera;
	
	switch (bbType)
	{
	case BillboardType::SCREEN:	
		AlignToScreen(camera);
		break;


	case BillboardType::WORLD:
		AlignToWorld(camera);
		break;

	case BillboardType::AXIS:
		AlignToAxis(camera);
		break;

	case BillboardType::NONE:
		break;
	}

}
void ComponentBillboard::AlignToScreen(ComponentCamera * camera)
{
	ComponentTransform* transform = GetOwner()->GetTransform();

	float4x4 viewMatrix = camera->GetViewMatrix();

	float3x3 rot = float3x3(viewMatrix.WorldX(), viewMatrix.WorldY(), viewMatrix.WorldZ());
	transform->SetRotationQuat(rot.ToQuat());
}

void ComponentBillboard::AlignToWorld(ComponentCamera * camera)
{
	ComponentTransform* transform = GetOwner()->GetTransform();

	//float3 front = (camera->GetPos() - transform->GetBoundingBox().CenterPoint()).Normalized(); // The object may not have BoundingBox (?)
	float3 front = (camera->GetPos() - transform->GetPosition()).Normalized();
	float3 up = camera->GetViewMatrix().WorldY();
	float3 right = up.Cross(front);
	up = front.Cross(right);

	float3x3 rot = float3x3(right, up, front);
	transform->SetRotationQuat(rot.ToQuat());
}

void ComponentBillboard::AlignToAxis(ComponentCamera * camera, int axisLocked)
{
	ComponentTransform* transform = GetOwner()->GetTransform();

	// TODO:: Maybe find another way to handle this && take other axis into account
	float3 front = (camera->GetPos() - transform->GetPosition()).Normalized();

	float3 up = float3::unitY;
	float3 right = up.Cross(front);
	front = right.Cross(up);

	float3x3 rot = float3x3(right, up, front);
	transform->SetRotationQuat(rot.ToQuat());
}

void ComponentBillboard::InspectorInfo()
{

	if (ImGui::CollapsingHeader("Billboard", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Billboard", &active); // Can't repeat checkbox name (!!)

		if (ImGui::Combo("Billboard Type", &currentSelected, "Screen Aligned\0World Aligned\0Axially Aligned\0None\0\0"))
		{
			bbType = (BillboardType)currentSelected;
		}

		if (bbType == BillboardType::AXIS)
		{
			ImGui::RadioButton("X Axis", &axisLocked, 0); ImGui::SameLine();
			ImGui::RadioButton("Y Axis", &axisLocked, 1); ImGui::SameLine();
			ImGui::RadioButton("Z Axis", &axisLocked, 2);
		}
	}
}

void ComponentBillboard::Save(JSON_Object * object, std::string name, bool saveScene, uint & countResources) const
{

	// TODO: Check Save / Load
	std::string tmp_bb;

	tmp_bb = name + "Type";
	json_object_dotset_number(object, tmp_bb.c_str(), (double)type);

	tmp_bb = name + "UUID";
	json_object_dotset_number(object, tmp_bb.c_str(), UUID);

	tmp_bb = name + "BillboardType";
	json_object_dotset_number(object, tmp_bb.c_str(), (double)bbType);


}

void ComponentBillboard::Load(const JSON_Object * object, std::string name)
{

	// TODO: Check Save / Load
	std::string tmp_bb;

	tmp_bb = name + "BillboardType";
	bbType = (BillboardType)(int)json_object_dotget_number(object, tmp_bb.c_str());
	currentSelected = (int)bbType;
}

