#include "ComponentMesh.h"
#include "Assimp/include/scene.h"
#include "csApp.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_MESH; 
}

ComponentMesh::~ComponentMesh()
{
	// Delete vertex
	if (vertex.buffer != nullptr)
	{
		delete[] vertex.buffer;
		vertex.buffer = nullptr;
	}
	
	// Delete Index
	if (index.buffer != nullptr)
	{
		delete[] index.buffer;
		index.buffer = nullptr;
	}

	// Delete Colors
	if (colors.buffer != nullptr)
	{
		delete[] colors.buffer;
		colors.buffer = nullptr;
	}

	// Delete Normals
	if (normals.buffer != nullptr)
	{
		delete[] normals.buffer;
		normals.buffer = nullptr;
	}

	// Delete Face Normals
	if (faceNormals.buffer != nullptr)
	{
		delete[] faceNormals.buffer;
		faceNormals.buffer = nullptr;
	}

	// Delete Vertex Normals
	if (vertexNormals.buffer != nullptr)
	{
		delete[] vertexNormals.buffer;
		vertexNormals.buffer = nullptr;
	}

	// Delete Texture Coords
	if (textureCoords.buffer != nullptr)
	{
		delete[] textureCoords.buffer;
		textureCoords.buffer = nullptr;
	}
}

void ComponentMesh::Update(float dt)
{
	Draw(); 
		
	if (App->renderer3D->drawNormals || drawVertexNormals)
		DrawNormals();

	if (App->renderer3D->drawVertexNormals || drawFaceNormals)
		DrawVertexNormals();
}

void ComponentMesh::Draw()
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (owner->HasComponent(ComponentType::C_MATERIAL))
	{
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(owner->FindComponent(ComponentType::C_MATERIAL));
		glColor3f(mat->GetColor().r, mat->GetColor().g, mat->GetColor().b);

		if (mat->GetTexture() != nullptr && textureCoords.buffer != nullptr && mat->isActive()) // Perhaps this conditional should be less strictive
		{
			// Enable Texture Coord array without a valid textureCoord buffer is boom
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

			// Texture Buffer
			glBindTexture(GL_TEXTURE_2D, mat->GetTexture()->id);

			// Texture Coords Buffer
			glBindBuffer(GL_ARRAY_BUFFER, textureCoords.id);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}
	}

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// Normals Buffer
	glBindBuffer(GL_ARRAY_BUFFER, normals.id);
	glNormalPointer(GL_FLOAT, 0, NULL);
	
	glPushMatrix(); 
	glMultMatrixf((GLfloat*)&(GetOwner()->GetTransform()->GetGlobalTransform().Transposed())); 

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);
	glDrawElements(GL_TRIANGLES, index.capacity, GL_UNSIGNED_INT, (void*)0);  // Carefull with this unsigned int
	
	// Reset 
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix(); 
	// Disable Clients
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::DrawNormals()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.f, 1.f, 0.f);

	glBindBuffer(GL_ARRAY_BUFFER, faceNormals.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, faceNormals.capacity);


	glColor3f(1.f, 1.f, 1.f);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::DrawVertexNormals()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.f, 0.f, 0.f); 

	glBindBuffer(GL_ARRAY_BUFFER, vertexNormals.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_LINES, 0, vertexNormals.capacity);

	
	glColor3f(1.f, 1.f, 1.f);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::LoadMeshVertices(aiMesh* mesh)
{
	vertex.capacity = mesh->mNumVertices;
	vertex.buffer = new float[vertex.capacity * 3];
	memcpy(vertex.buffer, mesh->mVertices, sizeof(float) * vertex.capacity * 3);

	//LOG("New mesh loaded with %d vertices", vertex.capacity);
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
	//LOG("New mesh loaded with %d indices", index.capacity);

	// ---------- Load Face Normals  ----------

	LoadMeshFaceNormals();
}

void ComponentMesh::LoadMeshNormals(aiMesh* mesh)
{
	normals.capacity = mesh->mNumVertices * 3;
	normals.buffer = new float[normals.capacity];
	memcpy(normals.buffer, mesh->mNormals, sizeof(float) * normals.capacity);
	//LOG("New mesh loaded with %d normals", normals.capacity);


	// ---------- Load Vertex Normals  ----------
	LoadMeshVertexNormals();
}

void ComponentMesh::LoadMeshFaceNormals()
{
	faceNormals.capacity = index.capacity * 2;
	faceNormals.buffer = new float[faceNormals.capacity];

	uint j = 0;
	uint normalSize = 1;

	for (uint i = 0; i < faceNormals.capacity; i += 6) //  vertexNormals.capacity * 2 (Origin vertex and Final Vertex)
	{
		// ----------
		// v1
		float v1x = vertex.buffer[index.buffer[j] * 3];
		float v1y = vertex.buffer[(index.buffer[j] * 3) + 1];
		float v1z = vertex.buffer[(index.buffer[j] * 3) + 2];

		float v2x = vertex.buffer[index.buffer[j + 1] * 3];
		float v2y = vertex.buffer[(index.buffer[j + 1] * 3) + 1];
		float v2z = vertex.buffer[(index.buffer[j + 1] * 3) + 2];

		float v3x = vertex.buffer[index.buffer[j + 2] * 3];
		float v3y = vertex.buffer[(index.buffer[j + 2] * 3) + 1];
		float v3z = vertex.buffer[(index.buffer[j + 2] * 3) + 2];

		float xMid = v1x + v2x + v3x;
		float yMid = v1y + v2y + v3y;
		float zMid = v1z + v2z + v3z;

		xMid /= 3;
		yMid /= 3;
		zMid /= 3;

		faceNormals.buffer[i] = xMid;
		faceNormals.buffer[i + 1] = yMid;
		faceNormals.buffer[i + 2] = zMid; 

		// ---------------
		// v2

		float3 v1(v1x, v1y, v1z);
		float3 v2(v2x, v2y, v2z);
		float3 v3(v3x, v3y, v3z);

		float3 u = v2 - v1;
		float3 v = v3 - v1;

		float normalX = (u.y * v.z) - (u.z * v.y);
		float normalY = (u.z * v.x) - (u.x * v.z);
		float normalZ = (u.x * v.y) - (u.y * v.x);

		float module = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
		float xNormal = xMid + (normalX / module) * normalSize;
		float yNormal = yMid + (normalY / module) * normalSize;
		float zNormal = zMid + (normalZ / module) * normalSize;

		faceNormals.buffer[i + 3] = xNormal;
		faceNormals.buffer[i + 4] = yNormal;
		faceNormals.buffer[i + 5] = zNormal; 

		j += 3;
	}
}

void ComponentMesh::LoadMeshVertexNormals()
{
	vertexNormals.capacity = normals.capacity * 2;
	vertexNormals.buffer = new float[vertexNormals.capacity];

	uint normalSize = 1;
	uint j = 0;
	for (uint i = 0; i < vertexNormals.capacity; i += 6) //  vertexNormals.capacity * 2 (Origin vertex and Final Vertex)
	{
		// v1
		memcpy(&vertexNormals.buffer[i], &vertex.buffer[j], sizeof(float));
		memcpy(&vertexNormals.buffer[i + 1], &vertex.buffer[j + 1], sizeof(float));
		memcpy(&vertexNormals.buffer[i + 2], &vertex.buffer[j + 2], sizeof(float));

		// v2
		vertexNormals.buffer[i + 3] = (vertex.buffer[j] + normals.buffer[j] * normalSize);
		vertexNormals.buffer[i + 4] = (vertex.buffer[j + 1] + normals.buffer[j + 1] * normalSize);
		vertexNormals.buffer[i + 5] = (vertex.buffer[j + 2] + normals.buffer[j + 2] * normalSize);
		j += 3;
	}
	//LOG("New mesh loaded with %d Vertex Normals", vertexNormals.capacity);
}

void ComponentMesh::LoadMeshColors(aiMesh* mesh, int index)
{
	colors.capacity = mesh->mNumVertices;
	colors.buffer = new float[colors.capacity * 4]; // (r,g,b,a) for each vertex
	memcpy(colors.buffer, mesh->mColors[index], sizeof(float) * colors.capacity * 4);
	//LOG("New mesh loaded with %d colors", colors.capacity);
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
	//LOG("New mesh loaded with %d Texture Coords", textureCoords.capacity);
}

void ComponentMesh::LoadMeshFromParShape(par_shapes_mesh * shape, BoundingBox* shapeBB)
{
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);

	
	if (shape->npoints > 0)
	{
		shapeBB->min = float3(shape->points[0], shape->points[1], shape->points[2]);
		shapeBB->max = float3(shape->points[0], shape->points[1], shape->points[2]);
	}

	// Vertex Buffer
	vertex.capacity = shape->npoints;
	vertex.buffer = new float[vertex.capacity * 3];
	memcpy(vertex.buffer, shape->points, sizeof(float) * vertex.capacity * 3);

	// Index Buffer
	index.capacity = shape->ntriangles * 3;
	index.buffer = new uint[index.capacity];
	memcpy(index.buffer, shape->triangles, sizeof(uint) * index.capacity);

	// Normal Buffer
	if(shape->normals != nullptr)
	{
		normals.capacity = shape->npoints * 3; 
		normals.buffer = new float[normals.capacity];
		memcpy(normals.buffer, shape->normals, sizeof(float) * normals.capacity);
	}

	// Texture Coords
	if (shape->tcoords != nullptr)
	{
		textureCoords.capacity = shape->npoints; 
		textureCoords.buffer = new float[textureCoords.capacity * 2];
		memcpy(textureCoords.buffer, shape->tcoords, sizeof(float) * textureCoords.capacity *2);
	}

	// Vertex Normals
	LoadMeshVertexNormals(); 

	// Face Normals
	LoadMeshFaceNormals(); 

	
	//Bounding Box
	for (uint i = 0; i < shape->npoints; i += 3)
	{
		float3 vec(shape->points[i], shape->points[i + 1], shape->points[i + 2]);

		shapeBB->min = shapeBB->min.Min(vec);
		shapeBB->max = shapeBB->max.Max(vec);
	}


	CreateMeshBuffers();
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
	else LOG("Error: There's no data to create a vertex buffer");


	// Index Buffer 

	if (index.buffer != nullptr)
	{
		glGenBuffers(1, &index.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * index.capacity, index.buffer, GL_STATIC_DRAW);
		LOG("Generated index buffer with ID %i and size %i ", index.id, index.capacity);
	}
	else LOG("Error: There's no data to create a index buffer");

	// Normals Buffer

	if (normals.buffer != nullptr)
	{
		glGenBuffers(1, &normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.capacity, normals.buffer, GL_STATIC_DRAW);
		LOG("Generated Normals buffer with ID %i and size %i ", normals.id, normals.capacity);
	}
	else LOG("Error: There's no data to create a index buffer");

	// Vertex Normals Buffer

	if (vertexNormals.buffer != nullptr)
	{
		glGenBuffers(1, &vertexNormals.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertexNormals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexNormals.capacity, vertexNormals.buffer, GL_STATIC_DRAW);
		LOG("Generated Vertex Normals Buffer buffer with ID %i and size %i ", vertexNormals.id, vertexNormals.capacity);
	}
	else LOG("Error: There's no data to create a Vertex Normals Buffer");
	
	// Face Normals Buffer

	if (faceNormals.buffer != nullptr)
	{
		glGenBuffers(1, &faceNormals.id);
		glBindBuffer(GL_ARRAY_BUFFER, faceNormals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * faceNormals.capacity, faceNormals.buffer, GL_STATIC_DRAW);
		LOG("Generated Face Normals Buffer buffer with ID %i and size %i ", faceNormals.id, faceNormals.capacity);
	}
	else LOG("Error: There's no data to create a Face Normals Buffer");

	// Texture Coords Buffer

	if (textureCoords.buffer != nullptr)
	{
		glGenBuffers(1, &textureCoords.id);
		glBindBuffer(GL_ARRAY_BUFFER, textureCoords.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.capacity * 2, textureCoords.buffer, GL_STATIC_DRAW);
		LOG("Generated Texture Coords buffer with ID %i and size %i ", textureCoords.id, textureCoords.capacity);
	}
	else LOG("Error: There's no data to create a Texture Coords buffer");


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
	if (ImGui::CollapsingHeader("Active Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &active);

		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetVertices());
		ImGui::Text("Total Indices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetIndices());
		ImGui::Text("Total UV:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetTextureCoords());
		ImGui::Text("Total Triangles:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", GetTriangles());

		ImGui::Checkbox("View Vertex Normals", &drawVertexNormals);
		ImGui::SameLine(); 
		ImGui::Checkbox("View Face Normals", &drawFaceNormals);
	}
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
	return index.capacity / 3;
}
