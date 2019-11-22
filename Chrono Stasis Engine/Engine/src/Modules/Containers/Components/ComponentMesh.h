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

	void LoadMeshVertices(aiMesh* mesh);
	void LoadMeshIndices(aiMesh* mesh);
	void LoadMeshNormals(aiMesh* mesh);
	void LoadMeshFaceNormals();
	void LoadMeshVertexNormals();
	void LoadMeshColors(aiMesh* mesh, int index);
	void LoadMeshTextureCoords(aiMesh* mesh, int index);

	void LoadMeshFromParShape(par_shapes_mesh* shape);

	void CreateMeshBuffers(); 


	void InspectorInfo();
	const uint GetIndices() const;
	const uint GetVertices() const;
	const uint GetTextureCoords() const;
	const uint GetTriangles() const;
	AABB GetAABB() const; 
	OBB GetOBBTransformed() const; 
	// Parser
	void Save(RJSON_Value* component) const;
	void Load(RJSON_Value* component);

public: 

	MeshInfo<uint> index;
	MeshInfo<float> vertex;
	MeshInfo<float> colors;
	MeshInfo<float> normals;
	MeshInfo<float> faceNormals; 
	MeshInfo<float> vertexNormals; 
	MeshInfo<float> textureCoords;

	bool drawVertexNormals = false;
	bool drawFaceNormals = false; 

	
};