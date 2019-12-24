#include "ComponentBillboard.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "csApp.h"


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
	ComponentTransform* transform = GetOwner()->GetTransform();

	// For the moment, debug with editor camera:
	ComponentCamera* camera = App->camera->fakeCamera;
	float4x4 viewMatrix = camera->GetViewMatrix();
	
	switch (bbtype)
	{
	case BillboardType::SCREEN:
	{
		float3x3 rot = float3x3(viewMatrix.WorldX(), viewMatrix.WorldY(), viewMatrix.WorldZ());
		transform->SetRotationQuat(rot.ToQuat());
	}
	break;


	case BillboardType::WORLD:
	{
		//float3 front = (camera->GetPos() - transform->GetBoundingBox().CenterPoint()).Normalized(); // The object may not have BoundingBox (?)
		float3 front = (camera->GetPos() - transform->GetPosition()).Normalized();
		float3 up = viewMatrix.WorldY();
		float3 right = up.Cross(front);
		up = front.Cross(right);

		float3x3 rot = float3x3(right, up, front);

		transform->SetRotationQuat(rot.ToQuat());
	}
		break;

	case BillboardType::AXIS:
	
		break;

	case BillboardType::NONE:
		break;
	}

}

void ComponentBillboard::InspectorInfo()
{

	if (ImGui::CollapsingHeader("Billboard", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Billboard", &active); // Can't repeat checkbox name (!!)

		if (ImGui::Combo("Billboard Type", &currentSelected, "Screen Aligned\0World Aligned\0Axially Aligned\0None\0\0"))
		{
			bbtype = (BillboardType)currentSelected;
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
	json_object_dotset_number(object, tmp_bb.c_str(), (double)bbtype);


}

void ComponentBillboard::Load(const JSON_Object * object, std::string name)
{

	// TODO: Check Save / Load
	std::string tmp_bb;

	tmp_bb = name + "BillboardType";
	bbtype = (BillboardType)(int)json_object_dotget_number(object, tmp_bb.c_str());
	currentSelected = (int)bbtype;
}

