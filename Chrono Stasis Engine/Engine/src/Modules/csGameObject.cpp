#include "csGameObject.h"
#include "csMesh.h"
#include "csApp.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::AssignMeshes(std::vector<Mesh*> mesh)
{
	meshes = mesh; 
}

void GameObject::AssignTexture(uint tex)
{
	std::vector<Mesh*>::const_iterator item = meshes.begin();
	for (item; item != meshes.end(); item++)
	{
		(*item)->AssignTexture(tex);
	}
}

void GameObject::Draw()
{
	std::vector<Mesh*>::const_iterator item = meshes.begin();
	for (item; item != meshes.end(); item++)
	{
		(*item)->Draw();
	}
}
