#include "csMesh.h"
#include "Assimp/include/scene.h"
#include "csApp.h"

Mesh::Mesh()
{
}

Mesh::Mesh(par_shapes_mesh * mesh)
{
	num_vertices = mesh->npoints; 
	num_indices = mesh->ntriangles * 3; 
	vertices = mesh->points;
	indices = (uint*)mesh->triangles; // TODO: Figure out how to solve 32 - 16 bites problem

	CreateMeshBuffers(); 
}

Mesh::Mesh(aiMesh * mesh)
{
	LoadMeshFromFBX(mesh); 
}

Mesh::~Mesh()
{
	delete[] indices;
	delete[] vertices;

	indices = nullptr; 
	vertices = nullptr;
}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)0);  // Carefull with this unsigned int
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::DrawNormals()
{
}

void Mesh::DrawVertexNormals()
{
}

void Mesh::LoadMeshVertices(aiMesh* mesh)
{
	vertex.capacity = mesh->mNumVertices;
	vertex.buffer = new float[vertex.capacity * 3];
	memcpy(vertex.buffer, mesh->mVertices, sizeof(float) * vertex.capacity * 3);

	LOG("New mesh loaded with %d vertices", vertex.capacity);
}

void Mesh::LoadMeshIndices(aiMesh* mesh)
{
	index.capacity = mesh->mNumFaces * 3;
	index.buffer = new uint[index.capacity]; 

	for (uint i = 0; i < mesh->mNumFaces; ++i)
	{
		if (mesh->mFaces[i].mNumIndices != 3) {
			LOG("WARNING, geometry face with != 3 indices!");
		}
	
		else memcpy(&index.buffer[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
	}
	LOG("New mesh loaded with %d indices", index.capacity);
}

void Mesh::LoadMeshNormals(aiMesh* mesh)
{
	normals.capacity = mesh->mNumFaces;
	normals.buffer = new float[normals.capacity * 3];
	memcpy(normals.buffer, mesh->mNormals, sizeof(float) * normals.capacity * 3);
	LOG("New mesh loaded with %d normals", normals.capacity);
}

void Mesh::LoadMeshColors(aiMesh* mesh, int index)
{
	colors.capacity = mesh->mNumVertices;
	colors.buffer = new float[colors.capacity * 4]; // (r,g,b,a) for each vertex
	memcpy(colors.buffer, mesh->mColors[index], sizeof(float) * colors.capacity * 4);
	LOG("New mesh loaded with %d colors", colors.capacity);
}

void Mesh::LoadMeshTextureCoords(aiMesh* mesh, int index)
{
	TextCoords.capacity = mesh->mNumVertices;
	TextCoords.buffer = new float[TextCoords.capacity * 2]; // 2 Coords (x,y) for each vertex
	memcpy(&TextCoords.buffer[index * 2], mesh->mTextureCoords[index], sizeof(float) * 2);
	LOG("New mesh loaded with %d Texture Coords", TextCoords.capacity);
}

void Mesh::LoadMeshFromFBX(aiMesh * mesh)
{
	LOG("Creating mesh from FBX");

	num_vertices = mesh->mNumVertices;
	vertices = new float[num_vertices * 3];
	memcpy(vertices, mesh->mVertices, sizeof(float) * num_vertices * 3);

	// copy faces
	if (mesh->HasFaces())
	{
		num_indices = mesh->mNumFaces * 3;
		indices = new uint[num_indices]; // assume each face is a triangle

		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}

			else memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}
}

void Mesh::CreateMeshBuffers()
{
	glGenBuffers(1, &id_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_indices, indices, GL_STATIC_DRAW);
}
