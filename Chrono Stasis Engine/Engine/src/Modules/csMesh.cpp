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
	glBegin(GL_LINES);

	glColor3f(0, 1, 0);
	uint normalSize = 5;

	int j = 0; 
	for (int i = 0; i < index.capacity; i += 3)
	{
		/*float xMid = vertex.buffer[i] + vertex.buffer[i + 3] + vertex.buffer[i + 6];
		float yMid = vertex.buffer[i + 1] + vertex.buffer[i + 4] + vertex.buffer[i + 7];
		float zMid = vertex.buffer[i + 2] + vertex.buffer[i + 5] + vertex.buffer[i + 8];*/


		// Center of triangle 

		float v1x = vertex.buffer[index.buffer[i] * 3] ; 
		float v1y = vertex.buffer[(index.buffer[i] * 3) + 1];
		float v1z = vertex.buffer[(index.buffer[i] * 3) + 2];
		
		float v2x = vertex.buffer[index.buffer[i + 1] * 3];
		float v2y = vertex.buffer[(index.buffer[i + 1] * 3) + 1];
		float v2z = vertex.buffer[(index.buffer[i + 1] * 3) + 2];
		
		float v3x = vertex.buffer[index.buffer[i + 2] * 3];
		float v3y = vertex.buffer[(index.buffer[i + 2] * 3) + 1];
		float v3z = vertex.buffer[(index.buffer[i + 2] * 3) + 2];


		float xMid = v1x + v2x + v3x; 
		float yMid = v1y + v2y + v3y; 
		float zMid = v1z + v2z + v3z; 

		xMid /= 3; 
		yMid /= 3; 
		zMid /= 3; 

		vec3 v1 = vec3(v1x, v1y, v1z); 
		vec3 v2 = vec3(v2x, v2y, v2z); 
		vec3 v3 = vec3(v3x, v3y, v3z);

		vec3 u = v2 - v1; 
		vec3 v = v3 - v1; 

		float normalX = (u.y * v.z) - (u.z * v.y);
		float normalY = (u.z * v.x) - (u.x * v.z);
		float normalZ = (u.x * v.y) - (u.y * v.x);
		
		float module = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
		normalX /= module; 
		normalY /= module; 
		normalZ /= module; 

		glVertex3f(xMid, yMid, zMid); 
		//glVertex3f(xMid + normals.buffer[j] * normalSize, yMid + normals.buffer[j + 1] * normalSize, zMid + normals.buffer[j + 2] * normalSize);
		glVertex3f(xMid + normalX * normalSize, yMid + normalY * normalSize, zMid + normalZ * normalSize);
		j += 3; 
	}

	glColor3f(0, 0, 0);

	glEnd();
}

void Mesh::DrawVertexNormals()
{
	glBegin(GL_LINES);
	uint normalSize = 10; 
	glColor3f(1, 0, 0);

	float n1x = 0;
	float n1y = 0;
	float n1z = 0;

	float v1x = 0;
	float v1y = 0;
	float v1z = 0;

	float v2x = 0;
	float v2y = 0;
	float v2z = 0;

	for (int i = 0; i < normals.capacity * 3; i += 3)
	{
		n1x = normals.buffer[i];			
		n1y = normals.buffer[i + 1];
		n1z = normals.buffer[i + 2];

		v1x = vertex.buffer[i];
		v1y = vertex.buffer[i + 1];
		v1z = vertex.buffer[i + 2];

		v2x = v1x + n1x * normalSize;
		v2y = v1y + n1y * normalSize;
		v2z = v1z + n1z * normalSize;

		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);

	}
	glColor3f(0, 0, 0);

	glEnd();
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
	normals.capacity = mesh->mNumFaces * 3;
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
