#include "csGameObject.h"
#include "csGlobals.h"
#include "csApp.h"
#include "src/Structure/HierarchyWindow.h"

// -----------
#include "csComponent.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

GameObject::GameObject()
{
	transform = new ComponentTransform(this);
	components.push_back(transform);
}

GameObject::GameObject(GameObject * parent) 
{
	SetParent(parent); 

	transform = new ComponentTransform(this);
	components.push_back(transform); 
}

GameObject::~GameObject()
{
	LOG("Deleting %s", name.c_str());

	if (App->scene->GetSelected() == this)
		App->scene->CleanSelected(); 

	std::list<GameObject*>::iterator it = childs.begin(); 

	for (it; it != childs.end(); ++it)
	{
		delete (*it); 
	}

	childs.clear(); 

	std::list<Component*>::iterator iter = components.begin();

	for (iter; iter != components.end(); ++iter)
	{
		delete (*iter);
	}

	components.clear();

}

void GameObject::Update(float dt)
{

	std::list<Component*>::const_iterator it = components.begin();

	for (it; it != components.end(); ++it)
	{
		if ((*it)->isActive())
			(*it)->Update(dt); 
	}

}

void GameObject::OnDraw()
{

	std::list<Component*>::const_iterator it = components.begin();

	for (it; it != components.end(); ++it)
	{
		if ((*it)->isActive())
			(*it)->OnDraw();
	}

}

void GameObject::OnDebugDraw()
{

	std::list<Component*>::const_iterator it = components.begin();

	for (it; it != components.end(); ++it)
	{
		if ((*it)->isActive())
			(*it)->OnDebugDraw();
	}

}


void GameObject::Enable()
{
	active = true; 
}

void GameObject::Disable()
{
	active = false;
}

void GameObject::RemoveChild(GameObject * child)
{
	childs.remove(child);
}

void GameObject::SetParent(GameObject * newParent)
{
	if (newParent == this || newParent == nullptr || newParent == parent)
		return; 

	if (parent != nullptr)
		parent->RemoveChild(this); 

	parent = newParent; 
	newParent->childs.push_back(this);
}

void GameObject::SetName(const char * name)
{
	this->name = name;
}

GameObject * GameObject::FindGreaterParent()
{
	if (this == App->scene->GetRoot())
	{
		LOG("Warning: You are trying to access to a higher parent being the Root")
		return this;
	}

	GameObject* ret = this; 

	while (ret->GetParent() != App->scene->GetRoot())
	{
		ret = ret->GetParent();
	}

	return ret; 
}

Component * GameObject::CreateComponent(ComponentType type)
{
	if (HasComponent(type))
	{
		LOG("Error adding component: Game Object %s already has this component", GetName()); 
		return nullptr; 
	}

	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::C_MESH:
		component = new ComponentMesh(this); 
		components.push_back(component); 
		break;

	case ComponentType::C_TRANSFORM:
		LOG("Transform Component Is already created for each Game Object.");
		break;

	case ComponentType::C_MATERIAL:
		if (this->HasComponent(ComponentType::C_MESH))
		{
			component = new ComponentMaterial(this);
			components.push_back(component);
		}
		else LOG("Error adding component: Can not create a 'Component Material' to a Game Object without a 'Component Mesh'. ")
		break;

	case ComponentType::C_CAMERA:
		component = new ComponentCamera(this); 
		components.push_back(component);
		break; 

	case ComponentType::C_LIGHT:

		break;

	default:
		break; 
	}

	return component;
}

bool GameObject::AssignComponent(Component * component)
{
	if (!HasComponent(component->GetType()))
	{
		if (component->GetOwner() != nullptr && component->GetOwner() != this)	// If has an owner and its not me
			component->GetOwner()->RemoveComponent(component); 

		component->SetOwner(this); 
		components.push_back(component);

		return true; 
	}
	
	LOG("Game Object Already has this component");
	return false; 
}

bool GameObject::HasComponent(ComponentType type)
{
	std::list<Component*>::const_iterator it = components.begin();
	for (it; it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			return true;
		}
	}

	return false;
}

Component * GameObject::FindComponent(ComponentType type)
{
	std::list<Component*>::const_iterator it = components.begin();
	for (it; it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			return (*it);
		}
	}

	return nullptr;
}

void GameObject::RemoveComponent(Component * component)
{
	components.remove(component);
}

void GameObject::FindComponentsInAllChilds(ComponentType type, std::vector<Component*>& compsfound)
{
	std::list<GameObject*>::iterator it = childs.begin();
	for (it; it != childs.end(); ++it)
	{
		Component* found = (*it)->FindComponent(type);
		if (found != nullptr)
			compsfound.push_back(found); 

		(*it)->FindComponentsInAllChilds(type, compsfound);
	}
}

bool GameObject::isActive() const
{
	return active;
}

bool GameObject::isStatic() const
{
	return staticGO;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

std::list<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

ComponentTransform * GameObject::GetTransform() const
{
	return transform;
}

uint GameObject::GetUUID() const
{
	return UUID;
}

void GameObject::DrawInspectorComponents()
{

	ImGui::Checkbox("-", &active); //GameObject active
	ImGui::SameLine();
	char buffer[256]; 
	strcpy(&buffer[0], name.c_str());
	if (ImGui::InputText("name", buffer, 256, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		SetName(buffer);
	ImGui::Checkbox("Static", &staticGO);
	if(ImGui::Button("Insert To Octree"))
		App->scene->octree->Insert(this);

	std::list<Component*>::const_iterator it = components.begin();
	for (it; it != components.end(); ++it)
	{
			(*it)->InspectorInfo();

		ImGui::Separator();
	}
	
}

void GameObject::Save(RJSON_Value* gos)
{
	RJSON_Value* gameObject = gos->CreateValue(rapidjson::kObjectType);

	gameObject->SetUint("UUID", GetUUID());
	gameObject->SetUint("Parent UUID", GetParent()->GetUUID());
	gameObject->SetString("Name", GetName());
	gameObject->SetBoolean("Active", isActive());
	gameObject->SetBoolean("Static", isStatic());

	RJSON_Value* comps = gameObject->CreateValue(rapidjson::kArrayType);

	for (auto& comp : components) // Serialize each GameObject Component
			comp->Save(comps);

	gameObject->AddValue("Components", *comps);
	gos->AddValue("", *gameObject);

	for (auto& child : childs) // Serialize GameObject Children
		child->Save(gos);
}
