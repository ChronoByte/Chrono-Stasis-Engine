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

void ComponentParticleSystem::OnPlay()
{
	particleSystem->ResetSystem(); 
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
	particleSystem->resMat = resMat;

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

				float radius = emmitter->GetRadius();
				if (ImGui::DragFloat("Radius", &radius, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetRadius(radius); }

				float outRadius = emmitter->GetOutRadius();
				if (ImGui::DragFloat("Outter Radius", &outRadius, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetOutRadius(outRadius); }
				ImGui::DragFloat("Distance", &emmitter->distance, 0.2f, 0.0f, FLT_MAX);
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

			float3 pos = particleSystem->emmitter.GetRelativePosition(); 
			if (ImGui::DragFloat3("Position", (float*)&pos)) { emmitter->SetRelativePosition(pos); }

			if (ImGui::TreeNodeEx("Burst"))
			{
				ImGui::Checkbox("Activate Burst", &emmitter->burst.active);
				ImGui::DragFloat("Time", &emmitter->burst.timeToBurst, 0.1f, 0, emmitter->GetMaxLife());
				ImGui::DragInt("Particles", &emmitter->burst.partsToInstantiate, 1.0f, 0, MAX_PARTICLES_TO_BURST);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		// ----------------------------------- Particle ----------------------------------------


		if (ImGui::TreeNodeEx("Particle", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Life Time", &particleSystem->particleInfo.maxLifeTime, 1.0f, 0.0f, FLT_MAX);
			ImGui::DragFloat("Speed", (float*)&particleSystem->particleInfo.speed, 0.2f);
			if (ImGui::Button("Chose Texture")) {
				App->editor->textureBrowser->SetActive(true); App->editor->textureBrowser->callback = this;
			}
			// Initial State || Final State
			if (ImGui::TreeNodeEx("Start State", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorPicker4("Color", (float*)&particleSystem->particleInfo.color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);
				ImGui::DragFloat("Size", (float*)&particleSystem->particleInfo.size, 0.1f, 0.0f, FLT_MAX);
				ImGui::DragFloat3("Gravity", (float*)&particleSystem->particleInfo.force);
				ImGui::Checkbox("Change Over Time", &particleSystem->particleInfo.changeOverLifeTime);

				ImGui::TreePop();
			}

			if (particleSystem->particleInfo.changeOverLifeTime)
			{
				if (ImGui::TreeNodeEx("Final State", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::ColorPicker4("Color", (float*)&particleSystem->endInfo.color,
						ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);
					ImGui::DragFloat("Size", (float*)&particleSystem->endInfo.size, 0.1f, 0.0f, FLT_MAX);
					ImGui::DragFloat3("Gravity", (float*)&particleSystem->endInfo.force);
					ImVec2 size = ImGui::GetItemRectSize();
					if (ImGui::Button("Equalize Values", size))	
						particleSystem->endInfo = particleSystem->particleInfo;
					ImGui::TreePop();
				}
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
			

			
			// Material
			if (resMat != nullptr)
			{

				if (ImGui::Combo("Material", &matTypeSelected, "None\0Texture\0Color\0\0"))
				{
					switch (matTypeSelected)
					{
					case 0: //None
					{
						App->editor->textureBrowser->callback->AssignResource(0);
						App->editor->textureBrowser->callback = nullptr;
						break;
					}
					case 1: //Texture
					{
						App->editor->textureBrowser->SwitchActive();
						App->editor->textureBrowser->callback = this;
						break;
					}
					case 2: //Color
					{
						break;
					}

					}
				}


				ImGui::Spacing();
				ImGui::Text(resMat->GetName());
				ImGui::Image((ImTextureID)resMat->gpu_id, ImVec2(ImVec2(PREVIEW_SIZE * 3, PREVIEW_SIZE * 3)), { 0,1 }, { 1,0 });
				ImGui::Text("Reference Counting: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (resMat == nullptr) ? 0 : resMat->CountReferences() - 1);
				ImGui::Spacing();
				ImGui::Text("Transparency: ");
				ImGui::SliderFloat(" ", &resMat->transparency, 0.0f, 1.0f);
				ImGui::Spacing();

				
				ImGui::Text("Blending Color: ");
				if (ImGui::Combo("Source", &eqTypeSelected, "ZERO\0ONE\0SRC_COLOR\0\0"))
				{

				}
					

				if (ImGui::Combo("Destination", &eqTypeSelected, "ZERO\0ONE\0SRC_COLOR\0\0"))
				{
				}
					
				ImGui::Spacing();

				ImGui::Text("Blend Equation: ");
				if (ImGui::Combo("Equation Mode", &eqTypeSelected, "ADD\0SUBTRACT\0REVERSE SUBTRACT\0\0"))
				{
					switch (eqTypeSelected)
					{
					case 0: //ADD
					{
						particleSystem->eqBlend = EquationBlendType::FUNC_ADD;
						break;
					}
					case 1: //SUBTRACT
					{
						particleSystem->eqBlend = EquationBlendType::FUNC_SUBTRACT;
						break;
					}
					case 2: //REVERSE SUBTRACT
					{
						particleSystem->eqBlend = EquationBlendType::FUNC_REVERSE_SUBTRACT;
						break;
					}

					}
				}
			
			}
			else
			{
				
				if (ImGui::Combo("Material", &matTypeSelected, "None\0Texture\0Color\0\0"))
				{
					switch (matTypeSelected)
					{
						case 0: //None
						{
							App->editor->textureBrowser->callback->AssignResource(0);
							App->editor->textureBrowser->callback = nullptr;
							break;
						}
						case 1: //Texture
						{
							App->editor->textureBrowser->SwitchActive();
							App->editor->textureBrowser->callback = this;
							break;
						}
						case 2: //Color
						{
							break;
						}

					}
				}
				
				ImGui::Spacing();
				ImGui::Text("No Texture");
				ImGui::Image((ImTextureID)2, ImVec2(ImVec2(PREVIEW_SIZE * 3, PREVIEW_SIZE * 3)), { 0,1 }, { 1,0 });
				ImGui::Text("Reference Counting: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (resMat == nullptr) ? 0 : resMat->CountReferences());
			}
			ImGui::TreePop();
		}
		

		if (ImGui::Button("Restart Particle System")) particleSystem->ResetSystem();

	}
}

ParticleSystem * ComponentParticleSystem::GetSystem() const
{
	return particleSystem;
}

void ComponentParticleSystem::Save(JSON_Object * object, std::string name, bool saveScene, uint & countResources) const
{

}

void ComponentParticleSystem::Load(const JSON_Object * object, std::string name)
{

}

