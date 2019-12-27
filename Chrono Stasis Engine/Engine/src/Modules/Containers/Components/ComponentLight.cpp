#include "ComponentLight.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "csApp.h"


ComponentLight::ComponentLight(GameObject* parent) : Component(parent)
{
	UUID = GenerateUUID();
	type = ComponentType::C_LIGHT;
	name = "Light";
}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::Update(float dt)
{
	ComponentTransform* transform = GetOwner()->GetTransform();

	if (transform != nullptr)
	{
		// calculate pos & dir
	}
	else
		LOG("LIGHT: Transform not working");
}

void ComponentLight::InspectorInfo()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Light", &active); // Can't repeat checkbox name (!!)

		
		if (ImGui::Combo("Light Type", &currentSelected, "Screen Aligned\0World Aligned\0Axially Aligned\0None\0\0"))
		{
			lightType = (LightType)currentSelected;
		}


		if (lightType == LightType::L_SPOT && lightType == LightType::L_POINT)
		{
			ImGui::Text("Fading");
			ImGui::DragFloat("Constant", (float*)&fading.x, 0.01f, 0.001f, 10.f);
			ImGui::DragFloat("Linear", (float*)&fading.y, 0.0001f, 0.0001f, 1.f);
			ImGui::DragFloat("Quadratic", (float*)&fading.z, 0.0001f, 0.0f, 1.f);
		}

		if (lightType == LightType::L_SPOT)
		{
			ImGui::Text("Angle");
			
		}

	}
}

void ComponentLight::Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
}

void ComponentLight::Load(const JSON_Object* object, std::string name)
{
}



void ComponentLight::SetPosition(const float3& pos)
{
	this->position = pos;
}

void ComponentLight::SetDirection(const float3& dir)
{
	this->direction = dir;
}

void ComponentLight::SetColor(const float3& col)
{
	this->color = col;
}

void ComponentLight::SetFading(const float3& fade)
{
	this->fading = fade;
}

float3 ComponentLight::GetPosition() const
{
	return position;
}

float3 ComponentLight::GetDirection() const
{
	return direction;
}

float3 ComponentLight::GetColor() const
{
	return color;
}

float3 ComponentLight::GetFading() const
{
	return fading;
}


//---------------------------------------------------
void ComponentLight::DrawLight() const
{
	
	switch (lightType) 
	{
		case LightType::L_DIRECTIONAL: DrawDirectional(); break;
		case LightType::L_POINT: DrawSpot(); break;
		case LightType::L_SPOT: DrawPoint(); break;

	}
	


}

void ComponentLight::DrawDirectional() const
{
}

void ComponentLight::DrawSpot() const
{
}

void ComponentLight::DrawPoint() const
{
}

void ComponentLight::DrawArea(const Circle& circle) const
{
}

