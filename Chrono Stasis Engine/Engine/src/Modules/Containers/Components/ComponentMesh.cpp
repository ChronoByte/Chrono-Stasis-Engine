#include "ComponentMesh.h"
#include "Assimp/include/scene.h"
#include "csApp.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_MESH; 
}

ComponentMesh::~ComponentMesh()
{
	if (vertex.buffer != nullptr)
	{
		delete[] vertex.buffer;
		vertex.buffer = nullptr;
	}
	
	if (index.buffer != nullptr)
	{
		delete[] index.buffer;
		index.buffer = nullptr;
	}
	if (colors.buffer != nullptr)
	{
		delete[] colors.buffer;
		colors.buffer = nullptr;
	}
	if (normals.buffer != nullptr)
	{
		delete[] normals.buffer;
		normals.buffer = nullptr;
	}
	if (textureCoords.buffer != nullptr)
	{
		delete[] textureCoords.buffer;
		textureCoords.buffer = nullptr;
	}
}

void ComponentMesh::Draw()
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, normals.id);
	glNormalPointer(GL_FLOAT, 0, NULL);
	
	if (tex != nullptr)
		glBindTexture(GL_TEXTURE_2D, tex->id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glBindBuffer(GL_ARRAY_BUFFER, textureCoords.id);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);
	glDrawElements(GL_TRIANGLES, index.capacity, GL_UNSIGNED_INT, (void*)0);  // Carefull with this unsigned int
	
	glDisableClientState(GL_NORMAL_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ComponentMesh::DrawNormals()
{
	glBegin(GL_LINES);

	glColor3f(0, 1, 0);
	uint normalSize = 2;

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

void ComponentMesh::DrawVertexNormals()
{
	glBegin(GL_LINES);
	uint normalSize = 2; 
	glColor3f(1, 0, 0);

	for (int i = 0; i < normals.capacity * 3; i += 3)
	{
		/*float v1x = vertex.buffer[i];
		float v1y = vertex.buffer[i + 1];
		float v1z = vertex.buffer[i + 2];

		float v2x = v1x + normals.buffer[i] * normalSize;
		float v2y = v1y + normals.buffer[i + 1] * normalSize;
		float v2z = v1z + normals.buffer[i + 2] * normalSize;*/

		glVertex3f(vertex.buffer[i], vertex.buffer[i + 1], vertex.buffer[i + 2]);
		glVertex3f(vertex.buffer[i] + normals.buffer[i] * normalSize, 
			vertex.buffer[i + 1] + normals.buffer[i + 1] * normalSize, 
			vertex.buffer[i + 2] + normals.buffer[i + 2] * normalSize);
	}

	glColor3f(0, 0, 0);

	glEnd();
}

void ComponentMesh::LoadMeshVertices(aiMesh* mesh)
{
	vertex.capacity = mesh->mNumVertices;
	vertex.buffer = new float[vertex.capacity * 3];
	memcpy(vertex.buffer, mesh->mVertices, sizeof(float) * vertex.capacity * 3);

	LOG("New mesh loaded with %d vertices", vertex.capacity);
}

void ComponentMesh::LoadMeshIndices(aiMesh* mesh)
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

void ComponentMesh::LoadMeshNormals(aiMesh* mesh)
{
	normals.capacity = mesh->mNumVertices * 3;
	normals.buffer = new float[normals.capacity];
	memcpy(normals.buffer, mesh->mNormals, sizeof(float) * normals.capacity);
	LOG("New mesh loaded with %d normals", normals.capacity);
}

void ComponentMesh::LoadMeshColors(aiMesh* mesh, int index)
{
	colors.capacity = mesh->mNumVertices;
	colors.buffer = new float[colors.capacity * 4]; // (r,g,b,a) for each vertex
	memcpy(colors.buffer, mesh->mColors[index], sizeof(float) * colors.capacity * 4);
	LOG("New mesh loaded with %d colors", colors.capacity);
}

void ComponentMesh::LoadMeshTextureCoords(aiMesh* mesh, int index)
{
	textureCoords.capacity = mesh->mNumVertices;
	textureCoords.buffer = new float[textureCoords.capacity * 2]; // 2 Coords (x,y) for each vertex
	uint j = 0; 
	for (uint i = 0; i < mesh->mNumVertices; ++i) {

		//there are two for each vertex
		memcpy(&textureCoords.buffer[j], &mesh->mTextureCoords[index][i].x, sizeof(float));
		memcpy(&textureCoords.buffer[j + 1], &mesh->mTextureCoords[index][i].y, sizeof(float));
		j += 2; 
	}
	LOG("New mesh loaded with %d Texture Coords", textureCoords.capacity);
}

void ComponentMesh::AssignTexture(TextureInfo * texture)
{
	this->tex = texture; 
}

void ComponentMesh::CreateMeshBuffers()
{
	// Vertices Buffer 

	if (vertex.buffer != nullptr)
	{
		glGenBuffers(1, &vertex.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.capacity * 3, vertex.buffer, GL_STATIC_DRAW);
		LOG("Generated vertex buffer with ID: %i and size %i", vertex.id, vertex.capacity);
	}
	else LOG("There's no data to create a vertex buffer");


	// Index Buffer 

	if (index.buffer != nullptr)
	{
		glGenBuffers(1, &index.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * index.capacity, index.buffer, GL_STATIC_DRAW);
		LOG("Generated index buffer with ID %i and size %i ", index.id, index.capacity);
	}
	else LOG("There's no data to create a index buffer");

	// Normals Buffer
	// To figure out if its needed a buffer for the normals. Believe so, but still to figure out how to do store it -> Normal Vertex

	if (normals.buffer != nullptr)
	{
		glGenBuffers(1, &normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.capacity, normals.buffer, GL_STATIC_DRAW);
		LOG("Generated Normals buffer with ID %i and size %i ", normals.id, normals.capacity);
	}
	else LOG("There's no data to create a index buffer");

	// Texture Coords Buffer

	if (textureCoords.buffer != nullptr)
	{
		glGenBuffers(1, &textureCoords.id);
		glBindBuffer(GL_ARRAY_BUFFER, textureCoords.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.capacity * 2, textureCoords.buffer, GL_STATIC_DRAW);
		LOG("Generated Texture Coords buffer with ID %i and size %i ", textureCoords.id, textureCoords.capacity);
	}
	else LOG("There's no data to create a Texture Coords buffer");


	// Colors Buffer
	if (colors.buffer != nullptr)
	{
		glGenBuffers(1, &colors.id);
		glBindBuffer(GL_ARRAY_BUFFER, colors.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.capacity * 4, colors.buffer, GL_STATIC_DRAW);
		LOG("Generated Colors buffer with ID %i and size %i ", colors.id, colors.capacity);
	}
	else LOG("There's no data to create Colors buffer");
}

void ComponentMesh::InspectorInfo()
{
	ImGui::Text("Total Vertices:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetVertices());
	ImGui::Text("Total Indices:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetIndices());
	ImGui::Text("Total UV:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetTextureCoords());
	ImGui::Text("Total Triangles:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetTriangles());

	//TODO: Add more info like normal checkbox, uv checkbox ...
}

const uint ComponentMesh::GetIndices() const
{
	return index.capacity;
}

const uint ComponentMesh::GetVertices() const
{
	return vertex.capacity;
}

const uint ComponentMesh::GetTextureCoords() const
{
	return textureCoords.capacity;
}

const uint ComponentMesh::GetTriangles() const
{
	return 0;
}
