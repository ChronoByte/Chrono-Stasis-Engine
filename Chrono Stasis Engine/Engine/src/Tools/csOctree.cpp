#include "csOctree.h"

#include "csGameObject.h"
#include "ComponentTransform.h"

// --------------------------------------- Octree Node ---------------------------------------

OctreeNode::OctreeNode(const SDL_Rect & zone)
	:zone(zone)
{
	objects.reserve(MAX_OBJECTS);

	for (uint i = 0; i < 4; ++i)
		childs[i] = nullptr; 

}

OctreeNode::~OctreeNode()
{
	for (uint i = 0; i < 4; ++i)
	{
		if (childs[i] != nullptr)
			delete childs[i];
	}

	// Should we clear our objects? 
}

void OctreeNode::Insert(GameObject * go)
{
	objects.push_back(go);

	// If it tops limit, then subdivide and redistribute
	if (objects.size() >= MAX_OBJECTS)
	{
		if (!isLeaf)
			Subdivide(); 

		DistributeInChilds(); 

	}
}

// Subdivides the node in 4 zones 
void OctreeNode::Subdivide()
{
	childs[0] = new OctreeNode(SDL_Rect({ zone.x, zone.y, zone.w / 2, zone.h / 2 }));
	childs[1] = new OctreeNode(SDL_Rect({ zone.x + zone.w / 2, zone.y, zone.w / 2, zone.h / 2 }));
	childs[2] = new OctreeNode(SDL_Rect({ zone.x, zone.y + zone.h / 2, zone.w / 2, zone.h / 2 }));
	childs[3] = new OctreeNode(SDL_Rect({ zone.x + zone.w / 2, zone.y + zone.h / 2, zone.w / 2, zone.h / 2 }));

	isLeaf = true;
}

// Redistribute all objects inside this node in its childs
void OctreeNode::DistributeInChilds()
{
	for (uint i = 0; i < objects.size(); ++i)
	{
		std::vector<int> childNum;
		GameObject* currentGo = objects.back();
		SDL_Rect goRect = currentGo->GetTransform()->GetBoundingBox2D();

		for (uint j = 0; j < 4; ++j)
		{
			// If its completely inside a child, push it there, delete it from here and there's no need to continue further
			if (IsInside(childs[i]->zone, goRect))
			{
				childs[i]->Insert(currentGo);
				currentGo = nullptr;
				objects.pop_back();
				break;
			}

			// If its intersecting, push the child "ID" to be assigned later (or not)
			if (Intersects(zone, goRect))
			{
				childNum.push_back(i);
			}
		}

		// Insert the GO in every node it fits in and delete it from here
		if (!childNum.empty() && childNum.size() < 4)
		{
			for (uint i = 0; i < childNum.size(); ++i)
			{
				childs[childNum[i]]->Insert(currentGo);
			}
			objects.pop_back();
		}

		// If it intersects with all 4 childs, we dont do anything, it stays at this current node
	}
}

int OctreeNode::CollectCandidates(std::vector<GameObject*> candidates, const SDL_Rect & rect)
{
	if (Intersects(zone, rect)) 
	{
		for (uint i = 0; i < objects.size(); ++i)
		{
			candidates.push_back(objects[i]);
		}
	}

	if (!isLeaf)
	{
		for (uint i = 0; i < 4; ++i)
		{
			childs[i]->CollectCandidates(candidates, rect); 
		}
	}

	return 0;
}

void OctreeNode::CollectZones(std::vector<OctreeNode*> candidates)
{
	candidates.push_back(this);

	if(!isLeaf)
	{
		for (uint i = 0; i < 4; ++i)
		{
			childs[i]->CollectZones(candidates);
		}
	}
}









// --------------------------------------- Actual OCTREE ---------------------------------------








Octree::Octree()
{
}

Octree::~Octree()
{
	ClearOctree(); 
}

void Octree::ClearOctree()
{
	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}
}

void Octree::SetBoundaries(const SDL_Rect & rect)
{
	ClearOctree(); 

	root = new OctreeNode(rect); 

}

void Octree::Insert(GameObject * go)
{
	if (root != nullptr)
	{
		SDL_Rect rect = go->GetTransform()->GetBoundingBox2D();
		if(Intersects(root->zone, rect))
			root->Insert(go); 
	}
}

int Octree::CollectCandidates(std::vector<GameObject*> candidates, const SDL_Rect & rect) const
{
	int tests = 1;
	if (root != nullptr && Intersects(root->zone, rect))
		tests = root->CollectCandidates(candidates, rect);

	return tests;
}

void Octree::CollectZones(std::vector<OctreeNode*> nodesCollected)
{
	if (root != NULL)
		root->CollectZones(nodesCollected);
}







// --------------------------------------------------------------------------------------------------

bool IsInside(const SDL_Rect & zone, const SDL_Rect & rect)
{
	return (zone.x <= rect.x &&
		zone.x + zone.w >= rect.x + rect.w &&
		zone.y <= rect.y &&
		zone.y + zone.h >= rect.y + rect.h);
}

bool Intersects(const SDL_Rect & zone, const SDL_Rect & rect)
{
	return SDL_HasIntersection(&zone, &rect) == SDL_TRUE;
}

