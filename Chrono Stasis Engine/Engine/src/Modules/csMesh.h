#pragma once

#include "csGlobals.h"

// TO DO: Add forward declaration so it doesnt
// have to include par_shapes
#include "par/par_shapes.h"

//struct par_shapes_mesh; 

class aiMesh; 

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
	void LoadMeshColors(aiMesh* mesh);
	void LoadMeshTextureCoords(aiMesh* mesh);

	void LoadMeshFromFBX(aiMesh* mesh); 
	void CreateMeshBuffers(); 

public: 

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;	

	MeshInfo<uint> index;
	MeshInfo<float> vertex;
	MeshInfo<float> colors;
	MeshInfo<float> normals;
	MeshInfo<float> TextCoords;

};