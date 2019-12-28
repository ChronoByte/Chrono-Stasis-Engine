#include "ComponentParticleSystem.h"
#include "Particle_System/csParticleSystem.h"
#include "csApp.h"
#include "csEditor.h"
#include "ComponentTransform.h"
#include "src/Structure/TextureBrowserWindow.h"

ComponentParticleSystem::ComponentParticleSystem(GameObject* parent) : Component(parent)
{
	UUID = GenerateUUID();
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
				if (ImGui::Combo("Source", &funcTypeSource, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0\0"))
				{
					switch (funcTypeSource)
					{
					case 0: 
					{
						particleSystem->funcBlendSource = FunctionBlendType::ZERO;
						break;
					}
					case 1: 
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE;
						break;
					}
					case 2: 
					{
						particleSystem->funcBlendSource = FunctionBlendType::SRC_COLOR;
						break;
					}
					case 3:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_SRC_COLOR;
						break;
					}
					case 4:
					{
						particleSystem->funcBlendSource = FunctionBlendType::DST_COLOR;
						break;
					}
					case 5:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_DST_COLOR;
						break;
					}
					case 6:
					{
						particleSystem->funcBlendSource = FunctionBlendType::SRC_ALPHA;
						break;
					}
					case 7:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_SRC_ALPHA;
						break;
					}
					case 8:
					{
						particleSystem->funcBlendSource = FunctionBlendType::DST_ALPHA;
						break;
					}
					case 9:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_DST_ALPHA;
						break;
					}
					case 10:
					{
						particleSystem->funcBlendSource = FunctionBlendType::CONSTANT_COLOR;
						break;
					}
					case 11:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_CONSTANT_COLOR;
						break;
					}
					case 12:
					{
						particleSystem->funcBlendSource = FunctionBlendType::CONSTANT_ALPHA;
						break;
					}
					case 13:
					{
						particleSystem->funcBlendSource = FunctionBlendType::ONE_MINUS_CONSTANT_ALPHA;
						break;
					}

					}
				}
					

				if (ImGui::Combo("Destination", &funcTypeDest, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0\0"))
				{
					switch (funcTypeDest)
					{
					case 0:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ZERO;
						break;
					}
					case 1:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE;
						break;
					}
					case 2:
					{
						particleSystem->funcBlendDest = FunctionBlendType::SRC_COLOR;
						break;
					}
					case 3:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_SRC_COLOR;
						break;
					}
					case 4:
					{
						particleSystem->funcBlendDest = FunctionBlendType::DST_COLOR;
						break;
					}
					case 5:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_DST_COLOR;
						break;
					}
					case 6:
					{
						particleSystem->funcBlendDest = FunctionBlendType::SRC_ALPHA;
						break;
					}
					case 7:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_SRC_ALPHA;
						break;
					}
					case 8:
					{
						particleSystem->funcBlendDest = FunctionBlendType::DST_ALPHA;
						break;
					}
					case 9:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_DST_ALPHA;
						break;
					}
					case 10:
					{
						particleSystem->funcBlendDest = FunctionBlendType::CONSTANT_COLOR;
						break;
					}
					case 11:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_CONSTANT_COLOR;
						break;
					}
					case 12:
					{
						particleSystem->funcBlendDest = FunctionBlendType::CONSTANT_ALPHA;
						break;
					}
					case 13:
					{
						particleSystem->funcBlendDest = FunctionBlendType::ONE_MINUS_CONSTANT_ALPHA;
						break;
					}

					}
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

	// --------------- General Info -------------------- //
	std::string tmp_ps;

	tmp_ps = name + "Type";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)type);
	//float4 tempColor = { color.r, color.g, color.b, color.a };

	tmp_ps = name + "UUID";
	json_object_dotset_number(object, tmp_ps.c_str(), UUID);

	// --------------- Particle System Start Info -------------------- //

	// Position
	tmp_ps = name + "Start.Position";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->particleInfo.position);
	// Rotation
	tmp_ps = name + "Start.Rotation";
	App->fs->json_array_dotset_float4(object, tmp_ps.c_str(), float4(particleSystem->particleInfo.rotation.x, particleSystem->particleInfo.rotation.y, particleSystem->particleInfo.rotation.z, particleSystem->particleInfo.rotation.w));
	// Velocity
	tmp_ps = name + "Start.Velocity";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->particleInfo.velocity);
	// Force
	tmp_ps = name + "Start.Force";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->particleInfo.force);
	// Speed
	tmp_ps = name + "Start.Speed";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->particleInfo.speed);
	// Color
	tmp_ps = name + "Start.Color";
	App->fs->json_array_dotset_float4(object, tmp_ps.c_str(), float4(particleSystem->particleInfo.color.x, particleSystem->particleInfo.color.y, particleSystem->particleInfo.color.z, particleSystem->particleInfo.color.w));
	// Size
	tmp_ps = name + "Start.Size";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->particleInfo.size);
	// LightColor
	tmp_ps = name + "Start.LightColor";
	App->fs->json_array_dotset_float4(object, tmp_ps.c_str(), float4(particleSystem->particleInfo.lightColor.x, particleSystem->particleInfo.lightColor.y, particleSystem->particleInfo.lightColor.z, particleSystem->particleInfo.lightColor.w));
	// MaxLifeTime
	tmp_ps = name + "Start.MaxLifeTime";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->particleInfo.maxLifeTime);
	// changeOverLifeTime
	tmp_ps = name + "Start.ChangeOverLifeTime";
	json_object_dotset_boolean(object, tmp_ps.c_str(), particleSystem->particleInfo.changeOverLifeTime);

	// --------------- Particle System End Info -------------------- //

	// Color
	tmp_ps = name + "End.Color";
	App->fs->json_array_dotset_float4(object, tmp_ps.c_str(), float4(particleSystem->endInfo.color.x, particleSystem->endInfo.color.y, particleSystem->endInfo.color.z, particleSystem->endInfo.color.w));
	// Size
	tmp_ps = name + "End.Size";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->endInfo.size);
	// LightColor
	tmp_ps = name + "End.LightColor";
	App->fs->json_array_dotset_float4(object, tmp_ps.c_str(), float4(particleSystem->endInfo.lightColor.x, particleSystem->endInfo.lightColor.y, particleSystem->endInfo.lightColor.z, particleSystem->endInfo.lightColor.w));
	// Force
	tmp_ps = name + "End.Force";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->endInfo.force);

	// --------------- Emitter Info -------------------- //
	
	// Shape
	tmp_ps = name + "Emmitter.Shape";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->emmitter.GetShape());
	// Radius
	tmp_ps = name + "Emmitter.Radius";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetRadius());
	// OutRadius
	tmp_ps = name + "Emmiter.OutRadius";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetOutRadius());
	// MaxLife
	tmp_ps = name + "Emmiter.MaxLife";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetMaxLife());
	// CurrentLife
	tmp_ps = name + "Emmiter.CurrentLife";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetCurrentLife());
	// SpawnRate
	tmp_ps = name + "Emmiter.SpawnRate";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetSpawnRate());
	// Delay
	tmp_ps = name + "Emmiter.Delay";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetDelay());
	// Loop
	tmp_ps = name + "Emmiter.Loop";
	json_object_dotset_boolean(object, tmp_ps.c_str(), particleSystem->emmitter.GetLoop());
	// Position
	tmp_ps = name + "Emmiter.Position";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetPosition());
	// RelativePosition
	tmp_ps = name + "Emmiter.RelativePosition";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetRelativePosition());
	// WorldPosition
	tmp_ps = name + "Emmiter.WorldPosition";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetWorldPosition());
	// Rotation
	tmp_ps = name + "Emmiter.Rotation";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetRotation());
	// Scale
	tmp_ps = name + "Emmiter.Scale";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetScale());
	
	

	// --------------- Material Resource Info -------------------- //

	if (currentResource != nullptr)
	{
		tmp_ps = name + "Resource Material Name";
		json_object_dotset_string(object, tmp_ps.c_str(), currentResource->GetName());

		tmp_ps = name + "Resource Material UUID";
		json_object_dotset_number(object, tmp_ps.c_str(), currentResource->GetUID());

		tmp_ps = name + "Resource Material File";
		json_object_dotset_string(object, tmp_ps.c_str(), currentResource->GetFile());

		tmp_ps = name + "Resource Material Path";
		json_object_dotset_string(object, tmp_ps.c_str(), currentResource->GetExportedFile());
	}
	else
	{
		tmp_ps = name + "Resource Material UUID";
		json_object_dotset_number(object, tmp_ps.c_str(), 0);
		tmp_ps = name + "Resource Material Path";
		json_object_dotset_string(object, tmp_ps.c_str(), "");
	}
}

void ComponentParticleSystem::Load(const JSON_Object * object, std::string name)
{

}

