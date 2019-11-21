#include "csGlobals.h"
#include "csApp.h"
#include "../Structure/HierarchyWindow.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "src/Structure/SceneViewWindow.h"
#include "csCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "Camera3D";
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init(JSON_Object* node)
{

	fakeCamera = new ComponentCamera(nullptr);

	fakeCamera->frustum.up = float3(json_object_dotget_number(node, "Up.x"), json_object_dotget_number(node, "Up.y"), json_object_dotget_number(node, "Up.z"));
	fakeCamera->frustum.front = float3(json_object_dotget_number(node, "Front.x"), json_object_dotget_number(node, "Front.y"), json_object_dotget_number(node, "Front.z"));

	fakeCamera->frustum.pos = float3(json_object_dotget_number(node, "position.x"), json_object_dotget_number(node, "position.y"), json_object_dotget_number(node, "position.z"));
	reference = float3(json_object_dotget_number(node, "reference.x"), json_object_dotget_number(node, "reference.y"), json_object_dotget_number(node, "reference.z"));
	LookAt(reference);

	cameraSpeed = json_object_get_number(node, "cameraSpeed");
	mouseSensitivity = json_object_get_number(node, "mouseSensitivity");
	wheelSensitivity = json_object_get_number(node, "wheelSensitivity");
	zoomSpeed = json_object_get_number(node, "zoomSpeed");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete fakeCamera; 
	fakeCamera = nullptr; 

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (cameraControls) {

		float3 newPos(0, 0, 0);
		float speed = cameraSpeed * dt;


		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 20.0f * dt;


		//------ROTATION (RIGHT CLICK) + FPS-LIKE MOVEMENT (RIGHT CLICK + AWSD)-----//
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) newPos.y -= speed;

			// FPS-LIKE MOVEMENT (RIGHT CLICK + AWSD)
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += fakeCamera->frustum.front * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= fakeCamera->frustum.front * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= fakeCamera->frustum.WorldRight()  * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += fakeCamera->frustum.WorldRight() * speed;

			// ROTATION MOVEMENT (RIGHT CLICK)
			orbit = false;
			reference = fakeCamera->frustum.pos - DistanceFromOrthonormalBasis();
		}

		//--------------ZOOM IN / ZOOM OUT (MOUSE WHEEL)-------------//
		if (App->input->GetMouseZ() > 0) newPos += fakeCamera->frustum.front * speed * zoomSpeed;
		if (App->input->GetMouseZ() < 0) newPos -= fakeCamera->frustum.front * speed * zoomSpeed;

		fakeCamera->frustum.pos += newPos;
		reference += newPos; 

		//----------ORBITATION MOVEMENT (LEFT CLICK + LALT)---------//
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
				orbit = true;
				fakeCamera->frustum.pos = reference + DistanceFromOrthonormalBasis(); 
			}

		}

		//---------------------FOCUS ON OBJECT (F)-------------------//
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)  FocusAtObject();


		//--------------- PANNING MOVEMENT ---------------//
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = App->input->GetMouseYMotion();


			if (dx != 0 || dy != 0)
			{
				float DeltaX = (float)dx * wheelSensitivity;
				float DeltaY = (float)dy * wheelSensitivity;

				newPos += fakeCamera->frustum.WorldRight() * DeltaX; 
				newPos += fakeCamera->frustum.up * DeltaY; 

				fakeCamera->frustum.pos += newPos; 
				reference += newPos; 

			}
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			MousePicking();
		}

	}
	

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::DrawMouseRay()
{
	if (ray.Length() != 0)
	{
		glBegin(GL_LINES);
		glLineWidth(2.5f);
		glColor3f(0, 0, 1.f);
		for (uint i = 0; i < 12; ++i)
		{
			glVertex3f(ray.a.x, ray.a.y, ray.a.z);
			glVertex3f(ray.b.x, ray.b.y, ray.b.z);
		}

		glColor3f(1.f, 1.f, 1.f);
		glEnd();
	}
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& Spot)
{
	reference = Spot;
	fakeCamera->LookAt(Spot); 
}


// -----------------------------------------------------------------

void ModuleCamera3D::FocusAtObject()
{
	GameObject* go_selected = App->scene->GetSelected();

	if (go_selected != nullptr)
	{
		const AABB* box = &go_selected->GetTransform()->GetBoundingBox();

		if (box->Size().IsZero())
			return;

		float3 center = box->CenterPoint();

		float3 direction = fakeCamera->frustum.pos - center;
		direction = direction.Normalized() * 10;

		fakeCamera->frustum.pos = float3(center.x + direction.x, center.y + direction.y, center.z + direction.z);
		LookAt(center); 

		/*float3 center = box->Centroid();
		float3 size = box->Size();

		float dirX = center.x;
		float dirY = center.y + size.y;
		float dirZ = center.z - size.z;

		Position.Set(dirX, dirY, dirZ);

		LookAt(vec3(center.x, center.y, center.z));*/

	}

}

float3 ModuleCamera3D::DistanceFromOrthonormalBasis()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float3 direction;
	if (orbit) 
		direction = fakeCamera->frustum.pos - reference;
	else 
		direction = reference - fakeCamera->frustum.pos;

	if (dx != 0)
	{
		float DeltaX = (float)dx * mouseSensitivity;

		fakeCamera->frustum.front = *(float3*)&(float4x4::RotateAxisAngle(float3::unitY, DeltaX) * float4(fakeCamera->frustum.front, 1.0f));
		fakeCamera->frustum.up = *(float3*)&(float4x4::RotateAxisAngle(float3::unitY, DeltaX) * float4(fakeCamera->frustum.up, 1.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * mouseSensitivity;

		float3 xAxis = fakeCamera->frustum.WorldRight(); 
		fakeCamera->frustum.front = *(float3*)&(float4x4::RotateAxisAngle(xAxis, DeltaY) * float4(fakeCamera->frustum.front, 1.0f));
		fakeCamera->frustum.up = *(float3*)&(float4x4::RotateAxisAngle(xAxis, DeltaY) * float4(fakeCamera->frustum.up, 1.0f));

		if (fakeCamera->frustum.up.y < 0.0f)
		{
			float3 xAxis = fakeCamera->frustum.WorldRight();
			fakeCamera->frustum.front = float3(0.0f, fakeCamera->frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			fakeCamera->frustum.up = fakeCamera->frustum.front.Cross(-xAxis).Normalized(); // This right here, this is live
		}
	}

	return -fakeCamera->frustum.front * direction.Length();
}

// -----------------------------------------------------------------
float4x4 ModuleCamera3D::GetViewMatrix()
{
	return fakeCamera->GetViewMatrix();
}

void ModuleCamera3D::MousePicking()
{
	/*float normalizedX = -1.0 + 2.0 * (float)App->input->GetMouseX() / (float)App->window->width;
	float normalizedY = 1.0 - 2.0 * (float)App->input->GetMouseY() / (float)App->window->height;*/
	
	float normalizedX = -1.0 + 2.0 *  App->editor->sceneView->GetMouseXInWindow() / App->editor->sceneView->GetWindowWidth();
	float normalizedY = 1.0 - 2.0 * App->editor->sceneView->GetMouseYInWindow() / App->editor->sceneView->GetWindowHeight();

	//LOG("Success: Normalized X = %f, Normalized Y = %f", normalizedX, normalizedY);
	//LOG("Corrected Mouse X: %f Mouse Y: %f", App->editor->sceneView->GetMouseXInWindow(), App->editor->sceneView->GetMouseYInWindow());
	ray = LineSegment(fakeCamera->frustum.UnProjectLineSegment(normalizedX, normalizedY));

	std::multimap<float, GameObject*> intersected;

	App->scene->CheckRayAgainstAABBS(App->scene->GetRoot(), ray, intersected); 

	LOG("AABBS hit ordered");
	for (std::multimap<float, GameObject*>::iterator it = intersected.begin(); it != intersected.end(); ++it)
	{
		LOG("%f -> %s", (*it).first, (*it).second->GetName());
	}

	GameObject* objectHit = App->scene->CheckRayAgainstTris(ray, intersected);

	App->scene->SetSelected(objectHit);

}

