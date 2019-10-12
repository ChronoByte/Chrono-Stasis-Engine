#include "csGameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt)
{

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
