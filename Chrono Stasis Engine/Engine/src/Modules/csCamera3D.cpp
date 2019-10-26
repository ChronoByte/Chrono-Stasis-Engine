#include "csGlobals.h"
#include "csApp.h"
#include "../Structure/HierarchyWindow.h"
#include "ComponentTransform.h"
#include "csCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "Camera3D";

	CalculateViewMatrix();

}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init(JSON_Object* node)
{
	X = vec3(json_object_dotget_number(node, "X.x"), json_object_dotget_number(node, "X.y"), json_object_dotget_number(node, "X.z"));
	Y = vec3(json_object_dotget_number(node, "Y.x"), json_object_dotget_number(node, "Y.y"), json_object_dotget_number(node, "Y.z"));
	Z = vec3(json_object_dotget_number(node, "Z.x"), json_object_dotget_number(node, "Z.y"), json_object_dotget_number(node, "Z.z"));
	
	Position = vec3(json_object_dotget_number(node, "position.x"), json_object_dotget_number(node, "position.y"), json_object_dotget_number(node, "position.z"));
	Reference = vec3(json_object_dotget_number(node, "reference.x"), json_object_dotget_number(node, "reference.y"), json_object_dotget_number(node, "reference.z"));
	LookAt(Reference);
	
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

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (cameraControls) {

		vec3 newPos(0, 0, 0);
		float speed = cameraSpeed * dt;


		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 20.0f * dt;


		//------ROTATION (RIGHT CLICK) + FPS-LIKE MOVEMENT (RIGHT CLICK + AWSD)-----//
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) newPos.y -= speed;

			// FPS-LIKE MOVEMENT (RIGHT CLICK + AWSD)
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			// ROTATION MOVEMENT (RIGHT CLICK)
			orbit = false;
			Reference = Position - DistanceFromOrthonormalBasis();
		}

		//--------------ZOOM IN / ZOOM OUT (MOUSE WHEEL)-------------//
		if (App->input->GetMouseZ() > 0) newPos -= Z * speed * zoomSpeed;
		if (App->input->GetMouseZ() < 0) newPos += Z * speed * zoomSpeed;

		Position += newPos;
		Reference += newPos;

		//----------ORBITATION MOVEMENT (LEFT CLICK + LALT)---------//
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
				orbit = true;
				Position = Reference + DistanceFromOrthonormalBasis();
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

				newPos += X * DeltaX;
				newPos += Y * DeltaY;

				Position += newPos;
				Reference += newPos;

			}
		}

		// Recalculate matrix -------------
		CalculateViewMatrix();
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::FocusAtObject()
{
	GameObject* go_selected = App->editor->hierarchy->GetSelected();

	if (go_selected != nullptr) 
	{
		const AABB* box = &go_selected->GetTransform()->GetBoundingBox();

		float3 center = box->CenterPoint();
		vec3 object = vec3(center.x, center.y, center.z);

		vec3 direction = Position - object;
		direction = normalize(direction) * 10;
	
		Position.Set(center.x + direction.x,center.y + direction.y,center.z + direction.z);
		LookAt(vec3(center.x,center.y,center.z));

		/*float3 center = box->Centroid();
		float3 size = box->Size();

		float dirX = center.x;
		float dirY = center.y + size.y;
		float dirZ = center.z - size.z;

		Position.Set(dirX, dirY, dirZ);
		
		LookAt(vec3(center.x, center.y, center.z));*/

	}

}

vec3 ModuleCamera3D::DistanceFromOrthonormalBasis()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();
	
	vec3 direction;
	if(orbit) direction = Position - Reference;
	else  direction = Reference - Position;

	
	if (dx != 0)
	{
		float DeltaX = (float)dx * mouseSensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * mouseSensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	
	 return Z * length(direction);
	
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}