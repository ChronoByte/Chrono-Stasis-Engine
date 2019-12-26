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
#include "ComponentBillboard.h"
#include "ComponentParticleSystem.h"
// -----------

GameObject::GameObject()
{
	UUID = GenerateUUID();
	transform = new ComponentTransform(this);
	components.push_back(transform);
}

GameObject::GameObject(GameObject * parent) 
{
	UUID = GenerateUUID();
	SetParent(parent); 

	transform = new ComponentTransform(this);
	components.push_back(transform); 
}

GameObject::GameObject(GameObject* GOparent, UID uid)
{
	this->UUID = uid;
	transform = new ComponentTransform(this);
	components.push_back(transform);

	if (GOparent->GetParent() == nullptr)
		GOparent->childs.push_back(this);

	this->parent = GOparent;
	//SetParent(parent);
}

GameObject::~GameObject()
{
	LOG("Deleting %s", name.c_str());

	if (App->scene->GetSelected() == this)
		App->scene->CleanSelected(); 

	// --------- Delete From Containers -----------

	if (staticGO)
		App->scene->RemoveFromStatic(this);
		//App->scene->RemoveFromOctree(this);
	else
		App->scene->RemoveFromDynamic(this); 
	
	// --------- Delete Everything its got -----------
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

	// --------------------------------------------
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

void GameObject::OnGameUpdate(float dt)
{
	LOG("Game Playing with dt: %f", dt); 
	std::list<Component*>::const_iterator it = components.begin();

	for (it; it != components.end(); ++it)
	{
		if ((*it)->isActive())
			(*it)->OnGameUpdate(dt);
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

void GameObject::SetStatic(bool stat)
{
	if (stat)
	{
		//App->scene->InsertInOctree(this);
		App->scene->PushToStatic(this); 
		App->scene->RemoveFromDynamic(this);
		staticGO = true;
	}
	else
	{
		//App->scene->RemoveFromOctree(this);
		App->scene->RemoveFromStatic(this);
		App->scene->PushToDynamic(this); 
		staticGO = false;
	}

	// Iteratively set childs
	std::vector<GameObject*> objects;
	objects.insert(objects.end(), childs.begin(), childs.end());

	while (!objects.empty())
	{
		GameObject* go = objects.back(); 
		objects.pop_back(); 
		objects.insert(objects.end(), go->childs.begin(), go->childs.end());

		if (stat == go->staticGO)
			continue; 

		if (stat)
		{
			//App->scene->InsertInOctree(go);
			App->scene->PushToStatic(go);
			App->scene->RemoveFromDynamic(go);
			go->staticGO = true;
		}
		else
		{
			//App->scene->RemoveFromOctree(go);
			App->scene->RemoveFromStatic(go);
			App->scene->PushToDynamic(go);
			go->staticGO = false;
		}
	}
}

void GameObject::SetActive(bool active)
{
	this->active = active;
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
		component = new ComponentTransform(this);
		components.push_back(component);
		//LOG("Transform Component Is already created for each Game Object.");
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

	case ComponentType::C_BILLBOARD:
		component = new ComponentBillboard(this);
		components.push_back(component);
		break; 

	case ComponentType::C_PARTICLE_SYSTEM:
		component = new ComponentParticleSystem(this);
		components.push_back(component);
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

UID GameObject::GetUUID() const
{
	return UUID;
}

int GameObject::GetComponentsNum() const
{
	return components.size();
}

int GameObject::GetChildrenNum() const
{
	return childs.size();
}

void GameObject::DrawInspectorComponents()
{

	ImGui::Checkbox("-", &active); //GameObject active
	ImGui::SameLine();
	char buffer[256]; 
	strcpy(&buffer[0], name.c_str());
	if (ImGui::InputText("name", buffer, 256, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		SetName(buffer);
	if (ImGui::Checkbox("Static", &staticGO))
		SetStatic(staticGO);


	std::list<Component*>::const_iterator it = components.begin();
	for (it; it != components.end(); ++it)
	{
			(*it)->InspectorInfo();

		ImGui::Separator();
	}
	
}


void GameObject::SaveComponents(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	int i = 0;
	for (auto& comp : components)
	{
		std::string temp = name + "Component " + std::to_string(i) + ".";
		comp->Save(object, temp, saveScene, countResources);
		i++;
	}
	
}

void GameObject::LoadComponents(const JSON_Object* object, std::string name, uint CompNums)
{
	// First Add All components by type
	for (int i = 0; i < CompNums; i++)
	{
		std::string comp = name + "Component " + std::to_string(i) + ".";
		std::string tmp_comp = comp + "Type";

		ComponentType type = (ComponentType)(int)json_object_dotget_number(object, tmp_comp.c_str());

		switch (type)
		{
		case ComponentType::C_NONE:
			break;
		case ComponentType::C_TRANSFORM:
			//this->CreateComponent(ComponentType::C_TRANSFORM);
			break;
		case ComponentType::C_MESH:
			this->CreateComponent(ComponentType::C_MESH);
			break;
		case ComponentType::C_MATERIAL:
			this->CreateComponent(ComponentType::C_MATERIAL);
			break;

		case ComponentType::C_CAMERA:
			this->CreateComponent(ComponentType::C_CAMERA);
			break;

		default:
			break;
		}
	}

	// Now Iterate All components and Load variables
	int i = 0;
	for (auto& comp : components)
	{
		std::string tmp_it = name + "Component " + std::to_string(i) + ".";
		comp->Load(object, tmp_it);
		i++;
	}
}

void GameObject::LoadGameObjectChild(GameObject* child)
{
	child->parent = this;
	childs.push_back(child);
}
