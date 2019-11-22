#include "csOctree.h"

#include "csGameObject.h"
#include "ComponentTransform.h"

// --------------------------------------- Octree Node ---------------------------------------

OctreeNode::OctreeNode(const AABB & zone, OctreeNode* parent)
	:zone(zone), parent(parent)
{
	objects.reserve(MAX_OBJECTS);

	for (uint i = 0; i < 8; ++i)
		childs[i] = nullptr; 

}

OctreeNode::~OctreeNode()
{
	for (uint i = 0; i < 8; ++i)
	{
		if (childs[i] != nullptr)
			delete childs[i];
	}

	// Should we clear our objects? 
}

void OctreeNode::Insert(GameObject * go)
{
	objects.push_back(go);
	LOG("Inserting GameObject to Octree"); 
	// If it tops limit, then subdivide and redistribute
	if (objects.size() >= MAX_OBJECTS)
	{
		if (isLeaf)
			Subdivide(); 

		DistributeInChilds(); 

	}
}

void OctreeNode::Remove(GameObject * go)
{
	for (uint i = 0; i < objects.size(); ++i)
	{
		if (objects[i] == go)
		{
			LOG("Success removing from octree: Found the object to be removed from the octree"); 
			// Remove it from the list of objects from that node
			objects.erase(objects.begin() + i); 

			// Try to delete brothers 
			TryToDeleteBrothers();
			 
			return; 
		}
	}

	if (!isLeaf)
	{
		for (uint i = 0; i < 8; ++i)
		{
			childs[i]->Remove(go);
		}
	}
}

void OctreeNode::TryToDeleteBrothers()
{
	if (parent != nullptr)
	{
		std::vector<GameObject*> brothersObjects(MAX_OBJECTS); 
		uint childsObjects = 0;
		for (uint i = 0; i < 8; ++i)
		{
			// Look in each brother, if the objects they have is greater than the max, we cant delete
			childsObjects += parent->childs[i]->objects.size();
			if (childsObjects > MAX_OBJECTS)
			{
				LOG("There are more than the max objects in the brothers node, can not delete childs");
				return;
			}
			//brothersObjects.insert(brothersObjects.end(), parent->childs[i]->objects.begin(), parent->childs[i]->objects.end());
		}
		LOG("You could have deleted the childs of the parent, its free real state");
	}
}

// Subdivides the node in 4 zones 
void OctreeNode::Subdivide()
{
	LOG("Subdividing Octree");
	float3 minPoint = zone.CenterPoint(); 
	float3 maxPoint = float3::zero;

	float3 halfSize = zone.HalfSize(); 

	// ---------

	// Upper half
	AABB abb;
	abb.SetNegativeInfinity(); 

	maxPoint = minPoint + float3(halfSize.x, halfSize.y, halfSize.z); 
	abb.Enclose(LineSegment(minPoint, maxPoint)); 
	childs[0] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(halfSize.x, halfSize.y, -halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[1] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(-halfSize.x, halfSize.y, halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[2] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(-halfSize.x, halfSize.y, -halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[3] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	// Lower half
	
	maxPoint = minPoint + float3(halfSize.x, -halfSize.y, halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[4] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(halfSize.x, -halfSize.y, -halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[5] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(-halfSize.x, -halfSize.y, halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[6] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	maxPoint = minPoint + float3(-halfSize.x, -halfSize.y, -halfSize.z);
	abb.Enclose(LineSegment(minPoint, maxPoint));
	childs[7] = new OctreeNode(abb, this);
	abb.SetNegativeInfinity();

	// ---------

	isLeaf = false;
}

// Redistribute all objects inside this node in its childs
void OctreeNode::DistributeInChilds()
{
	for (uint i = 0; i < objects.size(); ++i)
	{
		std::vector<int> childNum;
		childNum.reserve(8);

		GameObject* currentGo = objects.back();
		AABB goAABB = currentGo->GetTransform()->GetBoundingBox();

		for (uint j = 0; j < 8; ++j)
		{
			// If its completely inside a child, push it there, delete it from here and there's no need to continue further
			if (childs[j]->zone.Contains(goAABB))
			{
				LOG("Object is contained in one child in Redistribution");
				childs[j]->Insert(currentGo);
				currentGo = nullptr;
				objects.pop_back();
				break;
			}

			// If its intersecting, push the child "ID" to be assigned later (or not)
			if (childs[j]->zone.Intersects(goAABB))
			{
				childNum.push_back(i);
			}
		}

		// Insert the GO in every node it fits in and delete it from here
		if (!childNum.empty() && childNum.size() < 8)
		{
			LOG("Object is contained in more than one child in Redistribution");
			for (uint i = 0; i < childNum.size(); ++i)
			{
				childs[childNum[i]]->Insert(currentGo);
			}
			objects.pop_back();
		}

		// If it intersects with all 4 childs, we dont do anything, it stays at this current node
	}
}

void OctreeNode::CollectZones(std::vector<OctreeNode*>& candidates)
{
	candidates.push_back(this);

	if(!isLeaf)
	{
		for (uint i = 0; i < 8; ++i)
		{
			childs[i]->CollectZones(candidates);
		}
	}
}









// --------------------------------------- Actual OCTREE ---------------------------------------








Octree::Octree()
{
}

Octree::Octree(const AABB & zone)
{
	root = new OctreeNode(zone, nullptr);
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

void Octree::SetBoundaries(const AABB & zone)
{
	ClearOctree(); 

	root = new OctreeNode(zone, nullptr);

}

void Octree::Insert(GameObject * go)
{
	if (root != nullptr)
	{
		AABB goAABB = go->GetTransform()->GetBoundingBox();
		if(root->zone.Contains(goAABB))
			root->Insert(go); 
	}
}

void Octree::Remove(GameObject * go)
{
	if (root != nullptr)
	{
		root->Remove(go);
	}
}

void Octree::CollectZones(std::vector<OctreeNode*>& nodesCollected)
{
	if (root != nullptr)
		root->CollectZones(nodesCollected);
}



OctreeNode * Octree::GetRoot() const
{
	return root;
}



// --------------------------------------------------------------------------------------------------

//bool IsInside(const SDL_Rect & zone, const SDL_Rect & rect)
//{
//	return (zone.x <= rect.x &&
//		zone.x + zone.w >= rect.x + rect.w &&
//		zone.y <= rect.y &&
//		zone.y + zone.h >= rect.y + rect.h);
//}
//
//bool Intersects(const SDL_Rect & zone, const SDL_Rect & rect)
//{
//	return SDL_HasIntersection(&zone, &rect) == SDL_TRUE;
//}

