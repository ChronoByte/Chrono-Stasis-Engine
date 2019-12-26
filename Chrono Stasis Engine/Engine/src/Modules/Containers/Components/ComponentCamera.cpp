#include "ComponentCamera.h"
#include "csApp.h"
#include "csScene.h"

#include "GL/gl.h"
#include "csRenderer3D.h"
#include "csWindow.h"
#include "csFileSystem.h"

#include "ComponentTransform.h"
#include "csViewport.h"

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

void ComponentCamera::OnDebugDraw()
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

	// Vertex from initial point to near plane
	glVertex3f(frustum.pos.x, frustum.pos.y, frustum.pos.z);
	glVertex3f(frustum.pos.x + frustum.front.x * frustum.nearPlaneDistance, frustum.pos.y + frustum.front.y *  frustum.nearPlaneDistance, frustum.pos.z + frustum.front.z *  frustum.nearPlaneDistance);


	//glColor4f(1, 0.4f, 0.9f, 1);  // Pink
	glColor4f(1, 0.07f, 0.57f, 1);  // Pink
	//glColor3f(0.f, 1.f, 0.f); // Green

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
	
		// ------- Options --------- 

		ImGui::Checkbox("Frustum Culling", &culling);
		ImGui::Checkbox("Display Z-Buffer", &showZBuffer);
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

		ImGui::Separator();

		if (isMainCamera)
		{

			ImGui::Text("Preview");
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->renderTexture, { 150, 150 * aspectRatio }, { 0,1 }, { 1,0 });

			ImGui::SameLine();
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->zBufferTexture, { 150, 150 * aspectRatio }, { 0,1 }, { 1,0 });

			ImGui::Separator();

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
		ImGui::Text("Size:");
		ImGui::DragFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.1f, 0.1f, frustum.farPlaneDistance);
		ImGui::DragFloat("Far Plane Distance", &frustum.farPlaneDistance, 0.1f, frustum.nearPlaneDistance, 1000.f);
		ImGui::Separator();

		// ---------------------

		ImGui::Text("Background Color");
		ImGui::ColorEdit4("Background Color", bgColor, ImGuiColorEditFlags_Uint8);

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

	// Aspect ratio currently locked
	//aspectRatio = tanf(frustum.verticalFov * 0.5f) / tanf(frustum.horizontalFov * 0.5f);
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

void ComponentCamera::SetAspectRatio(const float & ratio)
{
	aspectRatio = ratio; 
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio); 
	//LOG("HFOV - %f - VFOV - %f", frustum.horizontalFov, frustum.verticalFov);
}

void ComponentCamera::SetColor(const Color & set)
{
	bgColor[0] = set.r;
	bgColor[1] = set.g;
	bgColor[2] = set.b;
	bgColor[3] = set.a;

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

const float* ComponentCamera::GetColor() const
{
	return bgColor;
}

float4 ComponentCamera::GetBGColor() const
{
	return float4(bgColor[0], bgColor[1], bgColor[2], bgColor[3] );
}

void ComponentCamera::Save(JSON_Object * object, std::string name, bool saveScene, uint & countResources) const
{
	std::string tmp_cam;

	tmp_cam = name + "Type";
	json_object_dotset_number(object, tmp_cam.c_str(), (double)type);

	tmp_cam = name + "Position"; 
	App->fs->json_array_dotset_float3(object, tmp_cam.c_str(), GetPos());

	tmp_cam = name + "Front";
	App->fs->json_array_dotset_float3(object, tmp_cam.c_str(), GetFrontVector());

	tmp_cam = name + "Up";
	App->fs->json_array_dotset_float3(object, tmp_cam.c_str(), GetUpVector());

	tmp_cam = name + "Color";
	App->fs->json_array_dotset_float4(object, tmp_cam.c_str(), GetBGColor());

	tmp_cam = name + "VFOV"; 
	json_object_dotset_number(object, tmp_cam.c_str(), (double)GetVerticalFOV());

	tmp_cam = name + "HFOV";
	json_object_dotset_number(object, tmp_cam.c_str(), (double)GetHorizontalFOV());

	tmp_cam = name + "Ratio";
	json_object_dotset_number(object, tmp_cam.c_str(), (double)aspectRatio);
}

void ComponentCamera::Load(const JSON_Object * object, std::string name)
{
	std::string tmp_cam;

	tmp_cam = name + "Position";
	frustum.pos = App->fs->json_array_dotget_float3_string(object, tmp_cam.c_str());

	tmp_cam = name + "Front";
	frustum.front = App->fs->json_array_dotget_float3_string(object, tmp_cam.c_str());

	tmp_cam = name + "Up";
	frustum.up = App->fs->json_array_dotget_float3_string(object, tmp_cam.c_str());
	
	tmp_cam = name + "Color";
	float4 bg = App->fs->json_array_dotget_float4_string(object, tmp_cam.c_str());

	tmp_cam = name + "VFOV";
	frustum.verticalFov = json_object_dotget_number(object, tmp_cam.c_str());

	tmp_cam = name + "HFOV";
	frustum.horizontalFov = json_object_dotget_number(object, tmp_cam.c_str());

	tmp_cam = name + "Ratio";
	aspectRatio = json_object_dotget_number(object, tmp_cam.c_str());

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
		bgColor[0] = 0.3f;
		bgColor[1] = 0.3f;
		bgColor[2] = 0.3f;
		bgColor[3] = 0.3f;
	}

	frustum.nearPlaneDistance = 1.f;
	frustum.farPlaneDistance = 500.f;

	frustum.verticalFov =  60 * DEGTORAD;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	aspectRatio = tanf(frustum.verticalFov * 0.5f) / tanf(frustum.horizontalFov * 0.5f);


}