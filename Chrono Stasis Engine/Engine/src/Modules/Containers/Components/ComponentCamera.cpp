#include "ComponentCamera.h"
#include "csApp.h"

#include "ComponentTransform.h"
ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_CAMERA;
	name = "Camera";

	SetInitially();
}

ComponentCamera::~ComponentCamera()
{
	if(owner != nullptr)
		LOG("Deleting component %s from %s", GetName(), owner->GetName());

	if(App->scene->GetMainCamera() == this)
		App->scene->ClearCamera();

}

void ComponentCamera::Update(float dt)
{

	UpdateTransform();

}

void ComponentCamera::OnDraw()
{

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
		ImGui::Text("Preview");

		// Here we could have the texture in miniature 

		ImGui::Separator(); 
	
		ImGui::Checkbox("Frustum Culling", &culling);
		if (ImGui::Checkbox("Set as Main Camera", &isMainCamera))
		{
			if (isMainCamera)
			{
				App->scene->SetMainCamera(this);
			}
			else
			{
				App->scene->ClearCamera();
			}
		}

		// ------- FOV --------- 

		ImGui::Text("FOV Axis");

		static int selected = 0; 
		ImGui::RadioButton("Vertical", &selected, 0); ImGui::SameLine();
		ImGui::RadioButton("Horizontal", &selected, 1);
		float verticalFOV = frustum.verticalFov * RADTODEG; 
		float horizontalFOV = frustum.horizontalFov * RADTODEG; 

		if (ImGui::DragFloat("Field Of View", (selected == 0) ? &verticalFOV : &horizontalFOV, 0.1f, 0.1f, 179.5f))
			UpdateRatio(selected == 0, verticalFOV * DEGTORAD, horizontalFOV * DEGTORAD);
		
		// ------- Planes --------- 

		ImGui::Separator(); 
		ImGui::DragFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.1f, 0.1f, frustum.farPlaneDistance);
		ImGui::DragFloat("Far Plane Distance", &frustum.farPlaneDistance, 0.1f, frustum.nearPlaneDistance, 1000.f);
		ImGui::Separator(); 

		// ---------------------
		// TODO: Add more customization 
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

void ComponentCamera::SetInitially()
{
	frustum.type = FrustumType::PerspectiveFrustum;

	if (owner != nullptr) 
	{
		UpdateTransform();
		App->scene->SetMainCamera(this);
		isMainCamera = true;
	}
	else
	{
		frustum.pos = float3(1.f, 6.f, 10.f);
		frustum.front = float3(0.f, 0.f, 1.f);
		frustum.up = float3(0.f, 1.f, 0.f);
	}

	frustum.nearPlaneDistance = 1.f;
	frustum.farPlaneDistance = 500.f;

	frustum.verticalFov = 60.f * DEGTORAD;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	aspectRatio = tanf(frustum.verticalFov * 0.5f) / tanf(frustum.horizontalFov * 0.5f);


}

void ComponentCamera::UpdateRatio(bool axisVertical, float verticalFOV, float horizontalFOV)
{
	if (axisVertical)
	{
		frustum.verticalFov = verticalFOV;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}
	else
	{
		frustum.horizontalFov = horizontalFOV;
		frustum.verticalFov = 2.f * atanf(tanf(frustum.horizontalFov * 0.5f) * ((float)App->window->height / (float)App->window->width));
	}
}

void ComponentCamera::LookAt(const float3 & lookatPos)
{
	float3 lookAt = lookatPos - frustum.pos;
	float3x3 dirMat = float3x3::LookAt(frustum.front, lookAt.Normalized(), frustum.up, float3::unitY);
	frustum.front = dirMat.MulDir(frustum.front).Normalized();
	frustum.up = dirMat.MulDir(frustum.up).Normalized();

}


// ---------------------- Sets -----------------------------


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

// ---------------------- Gets -----------------------------

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

float4x4 ComponentCamera::GetViewMatrix() const
{
	float4x4 viewMatrix = frustum.ViewMatrix();
	return viewMatrix.Transposed();
}

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

bool ComponentCamera::isCulling() const
{
	return culling;
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

