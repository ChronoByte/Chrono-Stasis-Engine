#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "csApp.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_TRANSFORM;
	name = "Transform";
	toRecalculateTransform = true; 
}

ComponentTransform::~ComponentTransform() 
{
	LOG("Deleting component transform from %s", owner->GetName());
}

void ComponentTransform::Update(float dt)
{


	if (toRecalculateTransform) 
	{
		CalculateTransformRecursively(); 
		toRecalculateTransform = false; 

		if (App->renderer3D->drawBoundingBox || drawBoundingBox)
			ForceParentBoundingBox();
	}
}


void ComponentTransform::OnDebugDraw()
{

	if (App->renderer3D->drawBoundingBox || drawBoundingBox)
		DrawBoundingBox();

}

void ComponentTransform::DrawBoundingBox()
{
	glBegin(GL_LINES);
	glLineWidth(2.0f);

	for (uint i = 0; i < boundingBox.aabb.NumEdges(); ++i)
	{
	
		// Draw AABB
		glColor3f(0, 1, 1);

		glVertex3f(boundingBox.aabb.Edge(i).a.x, boundingBox.aabb.Edge(i).a.y, boundingBox.aabb.Edge(i).a.z);
		glVertex3f(boundingBox.aabb.Edge(i).b.x, boundingBox.aabb.Edge(i).b.y, boundingBox.aabb.Edge(i).b.z);

		// Draw OBB
		glColor3f(1, 1, 1);

		glVertex3f(boundingBox.obb.Edge(i).a.x, boundingBox.obb.Edge(i).a.y, boundingBox.obb.Edge(i).a.z);
		glVertex3f(boundingBox.obb.Edge(i).b.x, boundingBox.obb.Edge(i).b.y, boundingBox.obb.Edge(i).b.z);
	
	}

	glColor3f(1, 1, 1);
	glEnd();
}


const float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_matrix;
}

const void ComponentTransform::SetLocalTransform(const float4x4& local)
{
	this->local_matrix = local;
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_matrix;
}

void ComponentTransform::CalculateTransformRecursively()
{
	GameObject* parent = owner->GetParent();

	if (parent == nullptr) 	// Root is trying to acces here // TODO: Handle root locking
		return;

	LOG("Recalculating Transform from: %s", owner->GetName());

	local_matrix = float4x4::FromTRS(position, rotation_quat, scale);
	global_matrix = parent->GetTransform()->GetGlobalTransform() * local_matrix;

	global_matrix.Decompose(globalPosition, globalRotation_quat, globalScale);

	UpdateBoundingBox();

	for (std::list<GameObject*>::const_iterator it = owner->childs.begin(); it != owner->childs.end(); ++it)
	{
		(*it)->GetTransform()->CalculateTransformRecursively();
	}
}

void ComponentTransform::UpdateBoundingBox()
{
	ComponentMesh* mesh = (ComponentMesh*)owner->FindComponent(ComponentType::C_MESH);

	// No mesh - no bbox
	if (mesh == nullptr)
		return; 

	boundingBox.obb = mesh->GetOBBTransformed(); 

	boundingBox.aabb.SetNegativeInfinity(); 
	boundingBox.aabb.Enclose(boundingBox.obb);
}

void ComponentTransform::UpdateParentBoundingBox()
{
	std::vector<Component*> meshesVector;

	owner->FindComponentsInAllChilds(ComponentType::C_MESH, meshesVector);
	boundingBox.aabb.SetNegativeInfinity();


	for (uint i = 0; i < meshesVector.size(); ++i)
	{
		ComponentMesh* mesh = (ComponentMesh*)meshesVector[i];
		boundingBox.obb = mesh->GetAABB();
		boundingBox.obb.Transform(mesh->GetOwner()->GetTransform()->GetGlobalTransform());
		boundingBox.aabb.Enclose(boundingBox.obb);
	}

	boundingBox.obb.SetNegativeInfinity(); 

	//boundingBox.obb = boundingBox.aabb;
	//boundingBox.obb.Transform(global_matrix);

	//boundingBox.aabb.SetNegativeInfinity();
	//boundingBox.aabb.Enclose(boundingBox.obb);
}

void ComponentTransform::ForceParentBoundingBox()
{
	if (owner == App->scene->GetRoot())
		return;

	GameObject* greaterParent = owner->FindGreaterParent();

	if (!greaterParent->HasComponent(ComponentType::C_MESH))
	{
		greaterParent->GetTransform()->UpdateParentBoundingBox();
		LOG("Forcing parent to Update BBox: ---- %s", greaterParent->GetName());
	}
}

const void ComponentTransform::SetPosition(const float3& pos)
{
	this->position = pos;
}

const void ComponentTransform::SetRotationEuler(const float3& euler)
{
	this->rotation_euler = euler;
	this->rotation_quat = Quat::FromEulerXYZ(euler.x * DEGTORAD, euler.y * DEGTORAD, euler.z * DEGTORAD);
	
}

const void ComponentTransform::SetRotationQuat(const Quat& quat)
{
	this->rotation_quat = quat;
	this->rotation_euler = rotation_quat.ToEulerXYZ() * RADTODEG;
}

const void ComponentTransform::SetScale(const float3& scale)
{
	this->scale = scale;
}

const float3 ComponentTransform::GetPosition() const
{
	return position;
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return rotation_euler;
}

const Quat ComponentTransform::GetRotationQuat() const
{
	return rotation_quat;
}

const float3 ComponentTransform::GetScale() const
{
	return scale;
}

const float3 ComponentTransform::GetGlobalPosition() const
{
	return globalPosition;
}

const float3 ComponentTransform::GetGlobalRotationEuler() const
{
	return globalRotation_euler;
}

const Quat ComponentTransform::GetGlobalRotationQuat() const
{
	return globalRotation_quat;
}

const float3 ComponentTransform::GetGlobalScale() const
{
	return globalScale;
}

const AABB ComponentTransform::GetBoundingBox() const
{
	return boundingBox.aabb;
}

SDL_Rect ComponentTransform::GetBoundingBox2D() const
{
	float3 maxZ = boundingBox.aabb.ExtremePoint(float3::unitZ);
	float3 maxX = boundingBox.aabb.ExtremePoint(float3::unitX);
	float width = boundingBox.aabb.ExtremePoint(float3::unitZ).Distance(boundingBox.aabb.ExtremePoint(-float3::unitZ));
	float height = boundingBox.aabb.ExtremePoint(float3::unitX).Distance(boundingBox.aabb.ExtremePoint(-float3::unitX));

	return SDL_Rect({ (int)maxZ.z, (int)maxX.x, (int)width, (int)height });
}

void ComponentTransform::SetupTransform(const float3& position, const float3& scale, const Quat& rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetRotationQuat(rotation); // euler setting up too!
}

const void ComponentTransform::SetBoundingBox(const AABB& bb)
{
	this->boundingBox.aabb = bb;
}

void ComponentTransform::InspectorInfo()
{

	if (ImGui::CollapsingHeader("Local Transformation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Position", (float*)&position, 0.5f))
		{
			SetPosition(position);
			toRecalculateTransform = true; 
		}

		if (ImGui::DragFloat3("Rotation", (float*)&rotation_euler, 0.5f))
		{ 
			SetRotationEuler(rotation_euler);
			toRecalculateTransform = true;
		}

		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.5f))
		{
			SetScale(scale);
			toRecalculateTransform = true;
		}

		if(!boundingBox.aabb.Size().IsZero())
			ImGui::Checkbox("View Bounding Box", &drawBoundingBox);
	}
	// TODO: More info like bounding box..
}

void ComponentTransform::Save(RJSON_Value* component) const
{
	RJSON_Value* transform = component->CreateValue(rapidjson::kObjectType);

	transform->SetVector3("Position", GetPosition());
	transform->SetVector3("Rotation", GetRotationEuler());
	transform->SetVector3("Scale", GetScale());

	component->AddValue("Transformation", *transform);
}

void ComponentTransform::Load(RJSON_Value* component)
{
}

