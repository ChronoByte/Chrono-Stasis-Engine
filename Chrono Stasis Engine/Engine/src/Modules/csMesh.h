#pragma once

#include "csGlobals.h"

// TO DO: Add forward declaration so it doesnt
// have to include par_shapes
#include "par/par_shapes.h"

//struct par_shapes_mesh; 

class aiMesh; 

class Mesh
{
public: 

	Mesh(); 
	Mesh(par_shapes_mesh* mesh); 
	Mesh(aiMesh* mesh); 
	~Mesh();

	void Draw();

	void LoadMeshFromFBX(aiMesh* mesh); 
	void CreateMeshBuffers(); 

public: 

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;	
};