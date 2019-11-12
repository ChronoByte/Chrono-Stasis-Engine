#include "ComponentTransform.h"
#include "csApp.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_TRANSFORM;
	name = "Transform";
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
	for (uint i = 0; i < 12; i++)
	{
		glBegin(GL_LINES);
		glLineWidth(1.0f);

		glColor3f(0, 1, 0);

		glVertex3f(bounding_box.Edge(i).a.x, bounding_box.Edge(i).a.y, bounding_box.Edge(i).a.z);
		glVertex3f(bounding_box.Edge(i).b.x, bounding_box.Edge(i).b.y, bounding_box.Edge(i).b.z);

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

	for (std::list<GameObject*>::const_iterator it = owner->childs.begin(); it != owner->childs.end(); ++it)
	{
		(*it)->GetTransform()->CalculateTransformRecursively();
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

const AABB ComponentTransform::GetBoundingBox() const
{
	return bounding_box;
}

void ComponentTransform::SetupTransform(const float3& position, const float3& scale, const Quat& rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetRotationQuat(rotation); // euler setting up too!
}

const void ComponentTransform::SetBoundingBox(const AABB& bb)
{
	this->bounding_box = bb;
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

		if(!bounding_box.Size().IsZero())
			ImGui::Checkbox("View Bounding Box", &drawBoundingBox);
	}
	// TODO: More info like bounding box..
}

