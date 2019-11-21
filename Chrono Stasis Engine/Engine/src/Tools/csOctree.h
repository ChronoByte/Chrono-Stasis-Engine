#pragma once

#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL_rect.h"

#define MAX_OBJECTS 2

class GameObject; 

class OctreeNode
{
public: 

	OctreeNode(const AABB& zone); 
	~OctreeNode();

	void Insert(GameObject* go);

	void Subdivide();
	void DistributeInChilds();

	int CollectCandidates(std::vector<GameObject*>& candidates, const AABB& rect);
	void CollectZones(std::vector<OctreeNode*>& candidates); 

	bool isLeaf = true; 
	std::vector<GameObject*> objects;
	OctreeNode* parent = nullptr; 
	OctreeNode* childs[8];
	AABB zone;
};

class Octree 
{
public: 


	Octree(); 
	Octree(const AABB& zone); 
	~Octree(); 

	void ClearOctree(); 
	void SetBoundaries(const AABB& rect);

	void Insert(GameObject* go);
	int CollectCandidates(std::vector<GameObject*>& candidates, const AABB& rect) const;

	void CollectZones(std::vector<OctreeNode*>& nodesCollected);

	OctreeNode* GetRoot() const; 

private: 

	OctreeNode* root = nullptr;
};

// Checkers 

//bool IsInside(const SDL_Rect& zone, const SDL_Rect& rect);
//bool Intersects(const SDL_Rect& zone, const SDL_Rect& rect); 
