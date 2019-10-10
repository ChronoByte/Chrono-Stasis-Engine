#pragma once

#include "csGlobals.h"

// TO DO: Add forward declaration so it doesnt
// have to include par_shapes
#include "par/par_shapes.h"

//struct par_shapes_mesh; 
#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH 100
class aiMesh; 
class TextureInfo; 

template<typename T>
struct MeshInfo
{
	uint id = 0u;
	uint capacity = 0u;
	T* buffer = nullptr;
};

class Mesh
{
public: 

	Mesh(); 
	Mesh(par_shapes_mesh* mesh); 
	Mesh(aiMesh* mesh); 
	~Mesh();

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