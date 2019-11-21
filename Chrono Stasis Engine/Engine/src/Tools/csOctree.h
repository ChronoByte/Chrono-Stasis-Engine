#pragma once

#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL_rect.h"
#include <map>

#define MAX_OBJECTS 2

class GameObject; 

class OctreeNode
{
public: 

	OctreeNode(const AABB& zone); 
	~OctreeNode();

	void Insert(GameObject* go);
	void Remove(GameObject* go); 

	void Subdivide();
	void DistributeInChilds();

	template <typename Type>
	int CollectCandidates(std::multimap<float, GameObject*>& candidates, const Type& primitive) const;

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
	void Remove(GameObject* go); 

	template <typename TYPE>
	int CollectCandidates(std::multimap<float, GameObject*>& candidates, const TYPE& primitive) const;

	void CollectZones(std::vector<OctreeNode*>& nodesCollected);

	OctreeNode* GetRoot() const; 

private: 

	OctreeNode* root = nullptr;
};

// Checkers 

//bool IsInside(const SDL_Rect& zone, const SDL_Rect& rect);
//bool Intersects(const SDL_Rect& zone, const SDL_Rect& rect); 

template<typename TYPE>
inline int Octree::CollectCandidates(std::multimap<float, GameObject*>& candidates, const TYPE & primitive) const
{
	if (root != nullptr && primitive.Intersects(root->zone))
		root->CollectCandidates(candidates, primitive);

	return 0;
}

template<typename Type>
inline int OctreeNode::CollectCandidates(std::multimap<float, GameObject*>& candidates, const Type & primitive) const
{
	if (primitive.Intersects(zone))
	{
		float minHit = FLOAT_INF;
		float maxHit = FLOAT_INF;
		for (uint i = 0; i < objects.size(); ++i)
		{
			if (primitive.Intersects(objects[i]->GetTransform()->GetBoundingBox(), minHit, maxHit))
			{
				GameObject* object = objects[i];  // Make this temporary variable to avoid error
				candidates.insert(std::pair<float, GameObject*>(minHit, object));
			}
		}

		if (!isLeaf)
		{
			for (uint i = 0; i < 8; ++i)
			{
				childs[i]->CollectCandidates(candidates, primitive);
			}
		}
	}

	return 0;
}
