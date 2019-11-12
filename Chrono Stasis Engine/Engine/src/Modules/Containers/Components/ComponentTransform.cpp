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
	if (App->renderer3D->drawBoundingBox || drawBoundingBox)
		 DrawBoundingBox(); 

	if (toRecalculateTransform) 
	{
		CalculateTransformRecursively(); 
		toRecalculateTransform = false; 
	}

	//local_matrix = float4x4::FromTRS(position, rotation_quat, scale);

	//// If its a child (not a parent and its not the root)
	//if (GetOwner()->GetParent() != nullptr && GetOwner()->GetParent() != App->scene->GetRoot())
	//	global_matrix = GetOwner()->GetParent()->GetTransform()->global_matrix * local_matrix;
	//else if(GetOwner() != App->scene->GetRoot())
	//	global_matrix = local_matrix; 


	/*LOG("______");
	LOG("Global Transform - Game Object: %s", GetOwner()->GetName());
	for (uint i = 0; i < 16; i += 4)
	{
		LOG("%.2f - %.2f - %.2f - %.2f", GetGlobalTransform().Transposed()[i],
			GetGlobalTransform().Transposed()[i + 1],
			GetGlobalTransform().Transposed()[i + 2],
			GetGlobalTransform().Transposed()[i + 3]
		);
	}*/
}

void ComponentTransform::DrawBoundingBox()
{

	for (uint i = 0; i < boundingBox.aabb.NumEdges(); ++i)
	{
		glBegin(GL_LINES);
		glLineWidth(2.0f);

		// Draw AABB
		glColor3f(0, 1, 1);

		glVertex3f(boundingBox.aabb.Edge(i).a.x, boundingBox.aabb.Edge(i).a.y, boundingBox.aabb.Edge(i).a.z);
		glVertex3f(boundingBox.aabb.Edge(i).b.x, boundingBox.aabb.Edge(i).b.y, boundingBox.aabb.Edge(i).b.z);

		// Draw OBB
		glColor3f(1, 1, 1);

		glVertex3f(boundingBox.obb.Edge(i).a.x, boundingBox.obb.Edge(i).a.y, boundingBox.obb.Edge(i).a.z);
		glVertex3f(boundingBox.obb.Edge(i).b.x, boundingBox.obb.Edge(i).b.y, boundingBox.obb.Edge(i).b.z);
	
		glColor3f(1, 1, 1);
		glEnd();
	}
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

	UpdateBoundingBox();

	for (std::list<GameObject*>::const_iterator it = owner->childs.begin(); it != owner->childs.end(); ++it)
	{
		(*it)->GetTransform()->CalculateTransformRecursively();
	}
}

void ComponentTransform::UpdateBoundingBox()
{
	ComponentMesh* mesh = (ComponentMesh*)owner->FindComponent(ComponentType::C_MESH);

	// TODO: Case on parents: where they have no mesh but still need to be focused and probably have a bbox
	if (mesh == nullptr)
		return; 

	boundingBox.obb = mesh->GetAABB(); 
	boundingBox.obb.Transform(global_matrix); 

	boundingBox.aabb.SetNegativeInfinity(); 
	boundingBox.aabb.Enclose(boundingBox.obb);
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

const AABB ComponentTransform::GetBoundingBox() const
{
	return boundingBox.aabb;
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
		if (ImGui::InputFloat3("Position", (float*)&position, 2))
		{
			SetPosition(position);
			toRecalculateTransform = true; 
		}

		if (ImGui::InputFloat3("Rotation", (float*)&rotation_euler, 2))
		{ 
			SetRotationEuler(rotation_euler);
			toRecalculateTransform = true;
		}

		if (ImGui::InputFloat3("Scale", (float*)&scale, 2))
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

