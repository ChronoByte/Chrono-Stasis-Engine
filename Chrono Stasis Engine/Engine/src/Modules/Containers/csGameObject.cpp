#include "csGameObject.h"
#include "csGlobals.h"
#include "csComponent.h"
#include "ComponentMesh.h"
GameObject::GameObject()
{
}

GameObject::GameObject(GameObject * parent) 
{
	SetParent(parent); 
}

GameObject::~GameObject()
{
	LOG("Deleting %s", name.c_str());
	parent = nullptr; 

	std::list<GameObject*>::const_iterator it = childs.begin(); 

	for (it; it != childs.end(); ++it)
	{
		delete (*it); 
	}

	childs.clear(); 

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

Component * GameObject::CreateComponent(ComponentType type)
{
	if (FindComponent(type))
		return nullptr; 

	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::C_MESH:
		component = new ComponentMesh(this); 
		components.push_back(component); 
		break;

	case ComponentType::C_TRANSFORM:

		break;

	case ComponentType::C_MATERIAL:

		break;

	case ComponentType::C_LIGHT:

		break;

	default:
		break; 
	}

	return component;
}

bool GameObject::AddComponent(Component * component)
{
	if (!FindComponent(component->GetType()))
	{
		components.push_back(component);
		return true; 
	}
	
	LOG("Game Object Already has this component");
	return false; 
}

bool GameObject::FindComponent(ComponentType type)
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

void GameObject::RemoveComponent(Component * component)
{
	components.remove(component);
}

bool GameObject::isActive() const
{
	return active;
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

void GameObject::DrawInspectorComponents()
{

	ImGui::Checkbox("go", &active); //GameObject active
	ImGui::SameLine();
	char* NonConstName = (char*)&name; //vale la pena romper el const del name para hacer el inputText y usar std::string name? 
	if (ImGui::InputText("name", NonConstName, 256, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		SetName(NonConstName);


	if (isActive()) {

		std::list<Component*>::const_iterator it = components.begin();
		for (it; it != components.end(); ++it)
		{
			if ((*it)->isActive()) //Component active
				(*it)->InspectorInfo();
			ImGui::Separator();
		}
	}
}
