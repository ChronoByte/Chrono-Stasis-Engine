#include "ComponentParticleSystem.h"
#include "Particle_System/csParticleSystem.h"
#include "csApp.h"
#include "csEditor.h"
#include "ComponentTransform.h"
#include "src/Structure/TextureBrowserWindow.h"

ComponentParticleSystem::ComponentParticleSystem(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_PARTICLE_SYSTEM;
	name = "Particle System";
	particleSystem = new ParticleSystem(); 
}

ComponentParticleSystem::~ComponentParticleSystem()
{
	if (particleSystem != nullptr)
	{
		delete particleSystem;
		particleSystem = nullptr; 
	}
}

void ComponentParticleSystem::Update(float dt)
{
	if (App->GetGameState() != GameState::ONSTOP)
		return;

	// TODO: Separate this 
	particleSystem->PreUpdate(dt);
	particleSystem->Update(dt);
	particleSystem->PostUpdate(dt);

	particleSystem->emmitter.SetPosition(owner->GetTransform()->GetPosition());
	particleSystem->emmitter.SetRotation(owner->GetTransform()->GetRotationQuat());
}

void ComponentParticleSystem::OnGameUpdate(float dt)
{
	// TODO: reset properly
	particleSystem->PreUpdate(dt);
	particleSystem->Update(dt);
	particleSystem->PostUpdate(dt);

	particleSystem->emmitter.SetPosition(owner->GetTransform()->GetPosition());
	particleSystem->emmitter.SetRotation(owner->GetTransform()->GetRotationQuat());
}

void ComponentParticleSystem::OnDraw()
{
	particleSystem->DrawParticles(); 
}

void ComponentParticleSystem::OnDebugDraw()
{
	if(drawEmmitter)
		particleSystem->DrawEmmitter();
}

void ComponentParticleSystem::InspectorInfo()
{
	ResourceTexture* resMat = (ResourceTexture*)currentResource;

	if (ImGui::CollapsingHeader("Particle System", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active System", &active); // Can't repeat checkbox name (!!)


		// ----------------------------------- Emmitter ----------------------------------------

		if (ImGui::TreeNodeEx("Emmitter", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ParticleEmmitter* emmitter = &particleSystem->emmitter; 

			ImGui::Checkbox("Draw Emmitter", &drawEmmitter);

			if (ImGui::TreeNode("Shape Options"))
			{
				if (ImGui::Combo("Shape", &shapeSelected, "Sphere\0Hemisphere\0Cube\0Cone\0Plane\0\0")) { emmitter->SetShape((Emmitter_Shape)shapeSelected); }

				// Radius,
				// Size, 
				// etc? 

				ImGui::TreePop();
			}

			float duration = emmitter->GetMaxLife();
			if (ImGui::DragFloat("Duration", &duration, 1.0F, 0.0F, FLT_MAX)) { emmitter->SetMaxLife(duration); }

			bool loop = emmitter->GetLoop();
			if (ImGui::Checkbox("Looping", &loop)) { emmitter->SetLoop(loop); }			

			float spawnRate = emmitter->GetSpawnRate();
			if (ImGui::DragFloat("Spawn Rate", &spawnRate, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetSpawnRate(spawnRate); }

			float3 pos = particleSystem->emmitter.GetPosition(); 
			if (ImGui::DragFloat3("Position", (float*)&pos)) { emmitter->SetPosition(pos); }

			ImGui::TreePop();
		}
		
		// ----------------------------------- Particle ----------------------------------------


		if (ImGui::TreeNodeEx("Particle", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Life Time", &particleSystem->particleInfo.maxLifeTime, 1.0f, 0.0f, FLT_MAX);
			ImGui::DragFloat("Speed", (float*)&particleSystem->particleInfo.speed, 0.2f);
			if(ImGui::Button("Chose Texture")) App->editor->textureBrowser->SetActive(true);
			// Initial State || Final State
			if (ImGui::TreeNodeEx("Start State", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorPicker4("Color", (float*)&particleSystem->particleInfo.color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);
				ImGui::DragFloat("Size", (float*)&particleSystem->particleInfo.size, 0.1f, 0.0f, FLT_MAX);
				ImGui::DragFloat3("Gravity", (float*)&particleSystem->particleInfo.force);

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Final State", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorPicker4("Color", (float*)&particleSystem->endInfo.color,
					ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);
				ImGui::DragFloat("Size", (float*)&particleSystem->endInfo.size, 0.1f, 0.0f, FLT_MAX);
				ImGui::DragFloat3("Gravity", (float*)&particleSystem->endInfo.force);

				ImGui::TreePop();
			}
			

			ImGui::TreePop();
		}
		
		// ----------------------------------- Renderer ----------------------------------------

		if (ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Billboarding ?
			if (ImGui::Combo("Billboard" , &bbTypeSelected, "Screen Aligned\0World Aligned\0Axially Aligned\0None\0\0"))
			{
				particleSystem->SetBillboardType((BillboardType)bbTypeSelected);
			}
			ImGui::TreePop();

			
			// Material
			if (resMat != nullptr)
			{
				ImGui::Text(resMat->GetName());
				ImGui::SameLine();
				if (ImGui::Button("Texture", ImVec2(50, 80)))
				{
					App->editor->textureBrowser->SwitchActive();
					
				}
			}
			else
			{
				ImGui::Text("No Texture");
				ImGui::SameLine();
				if (ImGui::Button("Select", ImVec2(60,20)))
				{
					App->editor->textureBrowser->SwitchActive();
					
				}
			}

		}

	}
}

void ComponentParticleSystem::Save(JSON_Object * object, std::string name, bool saveScene, uint & countResources) const
{

}

void ComponentParticleSystem::Load(const JSON_Object * object, std::string name)
{

}

