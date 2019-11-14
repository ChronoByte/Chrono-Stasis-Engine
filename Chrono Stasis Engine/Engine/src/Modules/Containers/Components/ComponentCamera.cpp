#include "ComponentCamera.h"
#include "csApp.h"

#include "ComponentTransform.h"
ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_CAMERA;
	name = "Camera";

	frustum.type = FrustumType::PerspectiveFrustum;
	ComponentTransform* transform = owner->GetTransform();
	frustum.pos = transform->GetGlobalPosition();
	frustum.front = transform->GetGlobalRotationQuat() * float3(0, 0, 1.f);
	frustum.up = transform->GetGlobalRotationQuat() * float3(0, 1, 0);

	frustum.nearPlaneDistance = 1.f;
	frustum.farPlaneDistance = 25.f;

	frustum.verticalFov = 60.f * DEGTORAD;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (float)(App->window->width / App->window->height));
	aspectRatio = tanf(frustum.verticalFov * 0.5f) / tanf(frustum.horizontalFov * 0.5f);
	App->scene->testCamera = this; 
}

ComponentCamera::~ComponentCamera()
{
	LOG("Deleting component transform from %s", owner->GetName());
}

void ComponentCamera::Update(float dt)
{

	UpdateTransform();
	DrawFrustum();

}

void ComponentCamera::UpdateTransform()
{
	ComponentTransform* transform = owner->GetTransform();

	frustum.pos = transform->GetGlobalPosition();
	frustum.up = transform->GetGlobalRotationQuat() * float3(0.f,1.f,0.f); // Up - y
	frustum.front = transform->GetGlobalRotationQuat() * float3(0.f,0.f,1.f); // Front - z
}

void ComponentCamera::DrawFrustum()
{
	glBegin(GL_LINES);
	glLineWidth(2.5f);
	glColor3f(1, 0.4f, 0.9f);

	for (uint i = 0; i < 12; ++i)
	{
		LineSegment edge = frustum.Edge(i);
		glVertex3f(edge.a.x, edge.a.y, edge.a.z);
		glVertex3f(edge.b.x, edge.b.y, edge.b.z);
	}

	glColor3f(1.f, 1.f, 1.f);
	glEnd(); 
}

void ComponentCamera::InspectorInfo()
{

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//const char* ratios[] = { "16:9", "1:1", "4:3", "21:9" };
		//static const char* currentRatio = ratios[0];            // Here our selection is a single pointer stored outside the object.
		//if (ImGui::BeginCombo("Aspect Ratios", currentRatio))   // The second parameter is the label previewed before opening the combo.
		//{
		//	for (int i = 0; i < (int)(sizeof(ratios) / sizeof(*ratios)); i++)
		//	{
		//		bool is_selected = (currentRatio == ratios[i]);

		//		if (ImGui::Selectable(ratios[i], is_selected))
		//			currentRatio = ratios[i];

		//		if (is_selected)
		//			ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		//	}
		//	ImGui::EndCombo();
		//}

		ImGui::DragFloat("Vertical Field Of View", &frustum.verticalFov, 0.1f);
		ImGui::DragFloat("Horizontal Field Of View", &frustum.horizontalFov, 0.1f);
		ImGui::Separator(); 
		ImGui::DragFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.1f);
		ImGui::DragFloat("Far Plane Distance", &frustum.farPlaneDistance, 0.1f);
		ImGui::Separator(); 
		ImGui::DragFloat("Aspect ratio", &aspectRatio, 0.1f);


	}
}

bool ComponentCamera::CheckAABBInsideFrustum(const OBB & myAabb)
{
	float3 pointsAABB[8];
	myAabb.GetCornerPoints(pointsAABB);

	for (uint plane = 0; plane < 6; ++plane)
	{
		int pointsIn = 8;
		for (uint point = 0; point < 8; ++point)
		{
			if (frustum.GetPlane(plane).IsOnPositiveSide(pointsAABB[point]))
				pointsIn--;
		}
		if (pointsIn == 0)
			return false; 
	}

	return true;
}

void ComponentCamera::SetPos(const float3 & pos)
{
	frustum.pos = pos;
}

void ComponentCamera::SetFrontVector(const float3 & front)
{
	frustum.front = front.Normalized(); 
}

void ComponentCamera::SetUpVector(const float3 & up)
{
	frustum.up = up.Normalized();
}

void ComponentCamera::SetFarPlaneDistance(const float & distance)
{
	frustum.farPlaneDistance = distance; 
}

void ComponentCamera::SetNearPlaneDistance(const float & distance)
{
	frustum.nearPlaneDistance = distance; 
}

void ComponentCamera::SetHorizontalFOV(const float & fov)
{
	frustum.horizontalFov = fov; 
}

void ComponentCamera::SetVerticalFOV(const float & fov)
{
	frustum.verticalFov = fov; 
}

float3 ComponentCamera::GetPos() const
{
	return frustum.pos;
}

float3 ComponentCamera::GetFrontVector() const
{
	return frustum.front;
}

float3 ComponentCamera::GetUpVector() const
{
	return frustum.up;
}

float ComponentCamera::GetFarPlaneDistance() const
{
	return frustum.farPlaneDistance;
}

float ComponentCamera::GetNearPlaneDistance() const
{
	return frustum.nearPlaneDistance;
}

float ComponentCamera::GetHorizontalFOV() const
{
	return frustum.horizontalFov;
}

float ComponentCamera::GetVerticalFOV() const
{
	return frustum.verticalFov;
}

void ComponentCamera::Save(RJSON_Value* component) const
{
	/*RJSON_Value* transform = component->CreateValue(rapidjson::kObjectType);

	transform->SetVector3("Position", GetPosition());
	transform->SetVector3("Rotation", GetRotationEuler());
	transform->SetVector3("Scale", GetScale());

	component->AddValue("Transformation", *transform);*/
}

void ComponentCamera::Load(RJSON_Value* component)
{
}

