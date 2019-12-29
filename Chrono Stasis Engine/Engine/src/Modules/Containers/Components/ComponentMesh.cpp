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

	// Unbind resource 

	if(currentResource != nullptr)
		currentResource->UnloadFromMemory(); 

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
	if (owner->logic.invisible)
		return; 

	ResourceMesh* meshRes = (ResourceMesh*)currentResource;


	if(App->scene->mainCamera != nullptr && App->scene->mainCamera->isCulling() 
		&& !App->scene->mainCamera->CheckAABBInsideFrustum(GetOBBTransformed()))
		return; 

	if (meshRes != nullptr) {

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		if (owner->HasComponent(ComponentType::C_MATERIAL))
		{
			ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(owner->FindComponent(ComponentType::C_MATERIAL));
			ResourceTexture* matRes = (ResourceTexture*)mat->GetCurrentResource();

			glColor3f(mat->GetColor().r, mat->GetColor().g, mat->GetColor().b);

			if (matRes != nullptr && meshRes->textureCoords.buffer != nullptr && mat->isActive()) // Perhaps this conditional should be less strictive
			{
				// Blending Texture
				/*glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

				// Enable Texture Coord array without a valid textureCoord buffer is boom
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				// Texture Buffer
				glBindTexture(GL_TEXTURE_2D, matRes->gpu_id);

				// Texture Coords Buffer
				glBindBuffer(GL_ARRAY_BUFFER, meshRes->textureCoords.id);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);

				if(mat->alpha_test)
				{
					glColor4f(1.0f, 1.0f, 1.0f, matRes->transparency);
					glEnable(GL_ALPHA_TEST);
					//glAlphaFunc(GL_GREATER, matRes->transparency);
				}
			}
		}

		// Vertex Buffer
		glBindBuffer(GL_ARRAY_BUFFER, meshRes->vertex.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		// Normals Buffer
		glBindBuffer(GL_ARRAY_BUFFER, meshRes->normals.id);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glPushMatrix();
		glMultMatrixf((GLfloat*) & (GetOwner()->GetTransform()->GetGlobalTransform().Transposed()));

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
}

void ComponentMesh::DrawNormals()
{
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;

	if (meshRes != nullptr) {

		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(0.f, 1.f, 0.f);

		glBindBuffer(GL_ARRAY_BUFFER, meshRes->faceNormals.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);


		glPushMatrix();
		glMultMatrixf((GLfloat*) & (GetOwner()->GetTransform()->GetGlobalTransform().Transposed()));

		glDrawArrays(GL_LINES, 0, meshRes->faceNormals.capacity);

		glPopMatrix();


		glColor3f(1.f, 1.f, 1.f);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void ComponentMesh::DrawVertexNormals()
{
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;
	
	if (meshRes != nullptr) 
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(1.f, 0.f, 0.f);

		glBindBuffer(GL_ARRAY_BUFFER, meshRes->vertexNormals.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);


		glPushMatrix();
		glMultMatrixf((GLfloat*) & (GetOwner()->GetTransform()->GetGlobalTransform().Transposed()));

		glDrawArrays(GL_LINES, 0, meshRes->vertexNormals.capacity);

		glPopMatrix();


		glColor3f(1.f, 1.f, 1.f);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}


void ComponentMesh::InspectorInfo()
{
	ResourceMesh* meshRes = (ResourceMesh*)currentResource;

	if (ImGui::CollapsingHeader("Active Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &active);

		ImGui::Text("Total Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetVertexNum());
		ImGui::Text("Total Indices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetIndexNum());
		ImGui::Text("Total UV:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetTextureCoordsSize());
		ImGui::Text("Total Triangles:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->GetTriangles());

		ImGui::Checkbox("View Vertex Normals", &drawVertexNormals);
		ImGui::SameLine(); 
		ImGui::Checkbox("View Face Normals", &drawFaceNormals);
		ImGui::Text("Reference Counting: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (meshRes == nullptr) ? 0 : meshRes->CountReferences());
	}
	//TODO: Add more info like normal checkbox, uv checkbox ...
}

AABB ComponentMesh::GetAABB() const
{
	ResourceMesh* res = (ResourceMesh*)currentResource; 

	if (res != nullptr)
	{
		AABB aabb;
		aabb.SetNegativeInfinity();
		aabb.Enclose((float3*)res->vertex.buffer, res->GetVertexNum());
		return aabb;
	}
}

OBB ComponentMesh::GetOBBTransformed() const
{
	OBB obb = GetAABB(); 
	obb.Transform(owner->GetTransform()->GetGlobalTransform());
	return obb;
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
		}
	}

}
