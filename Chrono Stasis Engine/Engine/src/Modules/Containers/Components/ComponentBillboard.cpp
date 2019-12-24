#include "ComponentBillboard.h"
#include "ComponentTransform.h"
#include "csApp.h"
#include "ResourceTexture.h"

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

	switch (bbtype)
	{
	case BillboardType::AXIS:
		// Do calcs

		break;

	case BillboardType::SCREEN:
		// Do calcs
		
		break;

	case BillboardType::WORLD:
		// Do calcs
		
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

		static int currentItem = (int)bbtype;
		if (ImGui::Combo("Billboard Type", &currentItem, "Screen Aligned\0World Aligned\0Axially Aligned\0None\0\0"))
		{
			bbtype = (BillboardType)currentItem; 
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
}

