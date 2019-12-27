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
	
	ComponentTransform* transform = GetOwner()->GetTransform();
	switch (bbType)
	{
	case BillboardType::SCREEN:	
		transform->SetRotationQuat(Billboard::AlignToScreen(camera));
		break;


	case BillboardType::WORLD:
		transform->SetRotationQuat(Billboard::AlignToWorld(camera, transform->GetPosition()));
		break;

	case BillboardType::AXIS:
		transform->SetRotationQuat(Billboard::AlignToAxis(camera, transform->GetPosition()));
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

