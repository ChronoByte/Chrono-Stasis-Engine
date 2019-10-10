#pragma once
#include "csGlobals.h"
#include <vector>

class Mesh;

class GameObject
{
public: 
	GameObject(); 
	~GameObject(); 

	void Update(float dt); 

	void AssignMeshes(std::vector<Mesh*> meshes); 
	void AssignTexture(uint tex);
	void Draw();

private:
	std::vector<Mesh*> meshes; 
	//TextureInfo* texture = nullptr;
};