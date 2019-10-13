#include "csGameObject.h"
#include "csGlobals.h"
#include "csComponent.h"
#include "ComponentMesh.h"
GameObject::GameObject()
{
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

void GameObject::RemoveChild(GameObject * child)
{
	childs.remove(child);
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * newParent)
{
	if (newParent == this)
		return; 

	if (parent != nullptr)
		parent->RemoveChild(this); 

	if (newParent != nullptr)
	{
		parent = newParent; 
		newParent->childs.push_back(this);
	}
}

void GameObject::SetName(const char * name)
{
	this->name = name;
}

Component * GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr; 

	switch (type)
	{
	case ComponentType::C_MESH:

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

void GameObject::AddComponent(Component * component)
{

	std::list<Component*>::const_iterator it = components.begin();
	for (it; it != components.end(); ++it)
	{
		if ((*it)->GetType() == component->GetType())
		{
			LOG("Game Object Already has this component");
			return; 
		}
	}

	components.push_back(component);
}

bool GameObject::isActive() const
{
	return active;
}

void GameObject::Enable()
{
	active = true; 
}

void GameObject::Disable()
{
	active = false;
}
