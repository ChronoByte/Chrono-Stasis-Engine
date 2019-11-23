#include "ComponentMesh.h"
#include "Assimp/include/scene.h"
#include "csApp.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	UUID = GenerateUUID();
	type = ComponentType::C_MESH; 
	name = "Mesh"; 
}

ComponentMesh::~ComponentMesh()
{
	LOG("Deleting component mesh from %s", owner->GetName());

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

void ComponentMesh::OnDraw()
{

	Draw();

}

void ComponentMesh::OnDebugDraw()
{
	if (App->renderer3D->drawNormals || drawFaceNormals)
		DrawNormals();

	if (App->renderer3D->drawVertexNormals || drawVertexNormals)
		DrawVertexNormals();
}

void ComponentMesh::Draw()
{
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;

	if(App->scene->mainCamera != nullptr && App->scene->mainCamera->isCulling() 
		&& !App->scene->mainCamera->CheckAABBInsideFrustum(GetOBBTransformed()))
		return; 

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (owner->HasComponent(ComponentType::C_MATERIAL))
	{
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(owner->FindComponent(ComponentType::C_MATERIAL));
		ResourceTexture* matRes = (ResourceTexture*)mat->GetCurrentResource();

		glColor3f(mat->GetColor().r, mat->GetColor().g, mat->GetColor().b);

		if (matRes != nullptr && meshRes->textureCoords.buffer != nullptr && mat->isActive()) // Perhaps this conditional should be less strictive
		{
			// Enable Texture Coord array without a valid textureCoord buffer is boom
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

			// Texture Buffer
			glBindTexture(GL_TEXTURE_2D, matRes->gpu_id);

			// Texture Coords Buffer
			glBindBuffer(GL_ARRAY_BUFFER, meshRes->textureCoords.id);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}
	}

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, meshRes->vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// Normals Buffer
	glBindBuffer(GL_ARRAY_BUFFER, meshRes->normals.id);
	glNormalPointer(GL_FLOAT, 0, NULL);
	
	glPushMatrix(); 
	glMultMatrixf((GLfloat*)&(GetOwner()->GetTransform()->GetGlobalTransform().Transposed())); 

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRes->index.id);
	glDrawElements(GL_TRIANGLES, meshRes->index.capacity, GL_UNSIGNED_INT, (void*)0);  // Carefull with this unsigned int
	
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
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.f, 1.f, 0.f);

	glBindBuffer(GL_ARRAY_BUFFER, meshRes->faceNormals.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glPushMatrix();
	glMultMatrixf((GLfloat*)&(GetOwner()->GetTransform()->GetGlobalTransform().Transposed()));

	glDrawArrays(GL_LINES, 0, meshRes->faceNormals.capacity);

	glPopMatrix();


	glColor3f(1.f, 1.f, 1.f);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::DrawVertexNormals()
{

	ResourceMesh* meshRes = (ResourceMesh*)currentResource;
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.f, 0.f, 0.f); 

	glBindBuffer(GL_ARRAY_BUFFER, meshRes->vertexNormals.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glPushMatrix();
	glMultMatrixf((GLfloat*)&(GetOwner()->GetTransform()->GetGlobalTransform().Transposed()));
	
	glDrawArrays(GL_LINES, 0, meshRes->vertexNormals.capacity);

	glPopMatrix();

	
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
	if(mesh->HasFaces())
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

void ComponentMesh::LoadMeshFromParShape(par_shapes_mesh * shape)
{
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);

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
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;

	if (ImGui::CollapsingHeader("Active Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &active);

		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetVertices());
		ImGui::Text("Total Indices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetIndices());
		ImGui::Text("Total UV:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetTextureCoords());
		ImGui::Text("Total Triangles:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetTriangles());

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

AABB ComponentMesh::GetAABB() const
{
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertex.buffer, vertex.capacity);
	return aabb;
}

OBB ComponentMesh::GetOBBTransformed() const
{
	OBB obb = GetAABB(); 
	obb.Transform(owner->GetTransform()->GetGlobalTransform());
	return obb;
}

void ComponentMesh::Save(RJSON_Value* component) const
{
	RJSON_Value* mesh = component->CreateValue(rapidjson::kObjectType);

	mesh->SetUint("UUID", UUID);
	component->AddValue("Geometry", *mesh);

	//TODO: MAKE MESH RESOURCE
		//mesh->SetString("FBX", );
		//mesh->SetString("Name Mesh",);
}

void ComponentMesh::Load(RJSON_Value* component)
{
}

void ComponentMesh::Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	std::string tmp_mesh;

	tmp_mesh = name + "Type";
	json_object_dotset_number(object, tmp_mesh.c_str(), (double)type);
	tmp_mesh = name + "UUID";
	json_object_dotset_number(object, tmp_mesh.c_str(), UUID);

	if (currentResource != nullptr)
	{
		//if (saveScene == false)
		//{
		//	// Save Info of Resource in Prefab (next we use this info for Reimport this prefab)
		//	std::string tmp_res;
		//	std::string temp = std::to_string(countResources++);

		//	tmp_res = "Info.Resources.Resource " + temp + ".UUID Resource";
		//	json_object_dotset_number(object, tmp_res.c_str(), currentResource->GetUID());
		//	tmp_res = "Info.Resources.Resource " + temp + ".Name";
		//	json_object_dotset_string(object, tmp_res.c_str(), currentResource->GetName());
		//}

		tmp_mesh = name + "Resource Mesh Name";
		json_object_dotset_string(object, tmp_mesh.c_str(), currentResource->GetName());

		tmp_mesh = name + "Resource Mesh UUID";
		json_object_dotset_number(object, tmp_mesh.c_str(), currentResource->GetUID());

		tmp_mesh = name + "Resource Mesh File";
		json_object_dotset_string(object, tmp_mesh.c_str(), currentResource->GetFile());

		tmp_mesh = name + "Resource Mesh Path";
		json_object_dotset_string(object, tmp_mesh.c_str(), currentResource->GetExportedFile());
	}
	else
	{
		tmp_mesh = name + "Resource Mesh UUID";
		json_object_dotset_number(object, tmp_mesh.c_str(), 0);
		tmp_mesh = name + "Resource Mesh Path";
		json_object_dotset_string(object, tmp_mesh.c_str(), "");
	}
}

void ComponentMesh::Load(const JSON_Object* object, std::string name)
{
	std::string tmp_mesh;

	tmp_mesh = name + "UUID";
	UUID = json_object_dotget_number(object, tmp_mesh.c_str());
	
	tmp_mesh = name + "Resource Mesh UUID";
	UID resUUID = json_object_dotget_number(object, tmp_mesh.c_str());

	tmp_mesh = name + "Resource Mesh Path";
	std::string exported_file = json_object_dotget_string(object, tmp_mesh.c_str());

	tmp_mesh = name + "Resource Mesh File";
	std::string file = json_object_dotget_string(object, tmp_mesh.c_str());

	tmp_mesh = name + "Resource Mesh Name";
	std::string name_file = json_object_dotget_string(object, tmp_mesh.c_str());


	if (resUUID > 0)
	{
		ResourceMesh* resMesh = (ResourceMesh*)App->resources->GetResource(resUUID);
		if (resMesh != nullptr)
		{
			this->AssignResource(resUUID);
			resMesh->LoadToMemory();
		}
		else 
		{
			resMesh = (ResourceMesh*)App->resources->CreateNewResource(Resource::R_MESH, resUUID, name_file.c_str(), file.c_str(), exported_file.c_str(), true);

			this->AssignResource(resUUID);
			resMesh->LoadToMemory();

			/*JSON_Value* config_file;
			JSON_Object* config;
			JSON_Object* config_node;

			config_file = json_parse_file(App->serialization->model_to_serialize.c_str());

			config = json_value_get_object(config_file);
			config_node = json_object_get_object(config, "Model");
			std::string file = json_object_dotget_string(config_node, "Info.Directory Model");

			int ResNum = json_object_dotget_number(config_node, "Info.Resources.Number of Resources");
			std::string name;

			for (int i = 0; i < ResNum; i++)
			{
				std::string tmp_res = "Info.Resources.Resource " + std::to_string(i) + ".UUID Resource";
				UID tmp_uid = json_object_dotget_number(config_node, tmp_res.c_str());

				if (tmp_uid == resUUID)
				{
					tmp_res = "Info.Resources.Resource " + std::to_string(i) + ".Name";
					name = json_object_dotget_string(config_node, tmp_res.c_str());
					break;
				}

			}*/

		}
	}

}
