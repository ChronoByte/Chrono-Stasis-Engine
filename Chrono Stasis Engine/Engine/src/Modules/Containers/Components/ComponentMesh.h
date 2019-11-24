#pragma once

#include "csGlobals.h"
#include "csComponent.h"

// TO DO: Add forward declaration so it doesnt
// have to include par_shapes
#include "par/par_shapes.h"
#include "MathGeoLib/include/Math/float3.h"
//struct par_shapes_mesh; 

class aiMesh; 
class TextureInfo; 
struct BoundingBox;
struct RJSON_Value;

#include "ResourceMesh.h"


class ComponentMesh : public Component
{
public: 

	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void OnDraw() override; 
	void OnDebugDraw() override; 

	void Draw();
	void DrawNormals(); 
	void DrawVertexNormals(); 

	void InspectorInfo();
	AABB GetAABB() const; 
	OBB GetOBBTransformed() const; 
	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

public: 

	bool drawVertexNormals = false;
	bool drawFaceNormals = false; 

};