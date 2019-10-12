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

	ComponentMesh();
	ComponentMesh(par_shapes_mesh* mesh);
	ComponentMesh(aiMesh* mesh);
	~ComponentMesh();

	void Draw();
	void DrawNormals(); 
	void DrawVertexNormals(); 

	void LoadMeshVertices(aiMesh* mesh);
	void LoadMeshIndices(aiMesh* mesh);
	void LoadMeshNormals(aiMesh* mesh);
	void LoadMeshColors(aiMesh* mesh, int index);
	void LoadMeshTextureCoords(aiMesh* mesh, int index);

	void AssignTexture(TextureInfo* texture);

	void CreateMeshBuffers(); 

public: 

	MeshInfo<uint> index;
	MeshInfo<float> vertex;
	MeshInfo<float> colors;
	MeshInfo<float> normals;
	MeshInfo<float> textureCoords;

	const TextureInfo* tex = nullptr;
};