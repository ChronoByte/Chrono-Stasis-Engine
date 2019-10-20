#pragma once

#include "csGlobals.h"
#include "csComponent.h"

// TO DO: Add forward declaration so it doesnt
// have to include par_shapes
#include "par/par_shapes.h"

//struct par_shapes_mesh; 

class aiMesh; 
class TextureInfo; 

template<typename T>
struct MeshInfo
{
	uint id = 0u;
	uint capacity = 0u;
	T* buffer = nullptr;
};

class ComponentMesh : public Component
{
public: 

	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Update(float dt) override; 

	void Draw();
	void DrawNormals(); 
	void DrawVertexNormals(); 

	void LoadMeshVertices(aiMesh* mesh);
	void LoadMeshIndices(aiMesh* mesh);
	void LoadMeshNormals(aiMesh* mesh);
	void LoadMeshFaceNormals(aiMesh * mesh);
	void LoadMeshVertexNormals(aiMesh * mesh);
	void LoadMeshColors(aiMesh* mesh, int index);
	void LoadMeshTextureCoords(aiMesh* mesh, int index);

	void LoadMeshFromParShape(par_shapes_mesh* shape);

	void CreateMeshBuffers(); 


	void InspectorInfo();
	const uint GetIndices() const;
	const uint GetVertices() const;
	const uint GetTextureCoords() const;
	const uint GetTriangles() const;

public: 

	MeshInfo<uint> index;
	MeshInfo<float> vertex;
	MeshInfo<float> colors;
	MeshInfo<float> normals;
	MeshInfo<float> faceNormals; 
	MeshInfo<float> vertexNormals; 
	MeshInfo<float> textureCoords;


	const TextureInfo* tex = nullptr;
};