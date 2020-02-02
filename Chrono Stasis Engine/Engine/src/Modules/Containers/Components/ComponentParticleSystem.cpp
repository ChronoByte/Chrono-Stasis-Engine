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

	if (currentResource != nullptr)
	{
		currentResource->UnloadFromMemory();
		currentResource = nullptr;
	}
}

void ComponentParticleSystem::Update(float dt)
{
	if (App->GetGameState() != GameState::ONSTOP || !particleSystem->isPlaying)
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
	particleSystem->isPlaying = true; 
}

void ComponentParticleSystem::OnGameUpdate(float dt)
{
	if (!particleSystem->isPlaying)
		return; 

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
	if(drawEmmitter && App->scene->GetSelected() == owner)
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

				switch (emmitter->GetShape())
				{
				case Emmitter_Shape::Sphere:
				{
					float radius = emmitter->GetRadius();
					if (ImGui::DragFloat("Radius", &radius, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetRadius(radius); }

					int zoneSelected = (int)emmitter->GetZone();
					if (ImGui::Combo("Emmission Zone", &zoneSelected, "Center\0Volume\0\0")) { emmitter->SetZone((Emmitter_Zone)zoneSelected); }
				}
					break;

				case Emmitter_Shape::Cone:
				{
					// Not inner radius yet

					float outRadius = emmitter->GetOutRadius();
					if (ImGui::DragFloat("Outter Radius", &outRadius, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetOutRadius(outRadius); }

					float distance = emmitter->GetDistance();
					if (ImGui::DragFloat("Distance", &distance, 0.2f, 0.0f, FLT_MAX)) { emmitter->SetDistance(distance); }
				}
					break; 

				}

				ImGui::TreePop();
			}

			float duration = emmitter->GetMaxLife();
			if (ImGui::DragFloat("Duration", &duration, 1.0F, 0.0F, FLT_MAX)) { emmitter->SetMaxLife(duration); }

			bool loop = emmitter->GetLoop();
			if (ImGui::Checkbox("Looping", &loop)) { emmitter->SetLoop(loop); }			

			float spawnRate = emmitter->GetSpawnRate();
			if (ImGui::DragFloat("Spawn Rate", &spawnRate, 0.2f, 0.01f, FLT_MAX)) { emmitter->SetSpawnRate(spawnRate); }

			float3 pos = particleSystem->emmitter.GetRelativePosition(); 
			if (ImGui::DragFloat3("Position", (float*)&pos)) { emmitter->SetRelativePosition(pos); }

			float3 rot = particleSystem->emmitter.GetRelativeRotation();
			if (ImGui::DragFloat3("Rotation", (float*)&rot)) { emmitter->SetRelativeRotation(rot); }

			if (ImGui::TreeNodeEx("Bursts"))
			{
				for (int i = 0; i < emmitter->bursts.size(); ++i)
				{			
					ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
					if (ImGui::TreeNodeEx((void*)(intptr_t)i, base_flags, "Burst %i", i))
					{
						ImGui::DragFloat("Time", &emmitter->bursts[i].timeToBurst, 0.1f, 0, emmitter->GetMaxLife());
						ImGui::DragInt("Particles", &emmitter->bursts[i].partsToInstantiate, 1.0f, 0, MAX_PARTICLES_TO_BURST);
						if(ImGui::Button("Remove")) emmitter->RemoveBurst(i);
						ImGui::TreePop();

					}
				}

				if (ImGui::Button("Add Burst")) { emmitter->bursts.push_back(Burst()); }

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		// ----------------------------------- Particle ----------------------------------------


		if (ImGui::TreeNodeEx("Particle", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Life Time", &particleSystem->particleInfo.maxLifeTime, 1.0f, 0.0f, FLT_MAX);
			ImGui::DragFloat("Speed", (float*)&particleSystem->particleInfo.speed, 0.2f);

			if (ImGui::RadioButton("Global", &transformSelected, 0)) { particleSystem->particleInfo.globalTransform = true; }
			ImGui::SameLine();
			if (ImGui::RadioButton("Local", &transformSelected, 1)) { particleSystem->particleInfo.globalTransform = false; }
			
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
		
			// Add texture
			if (ImGui::Button("Choose Texture"))
			{
				App->editor->textureBrowser->SetActive(true);
				App->editor->textureBrowser->callback = this;
			}

			if (resMat != nullptr)
			{
				// If there is a texture, option to remove it
				ImGui::SameLine();
				if (ImGui::Button("Remove texture"))
				{
					App->editor->textureBrowser->callback = this;
					App->editor->textureBrowser->callback->AssignResource(0);
					App->editor->textureBrowser->callback = nullptr;
				}

				ImGui::Spacing();
				ImGui::Text(resMat->GetName());
				ImGui::Image((ImTextureID)resMat->gpu_id, ImVec2(ImVec2(PREVIEW_SIZE * 3, PREVIEW_SIZE * 3)), { 0,1 }, { 1,0 });
				ImGui::Text("Reference Counting: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (resMat == nullptr) ? 0 : resMat->CountReferences() - 1);
				ImGui::Spacing();
				ImGui::Text("Transparency: ");
				ImGui::SliderFloat(" ", &particleSystem->particleInfo.color.w, 0.0f, 1.0f);
				ImGui::Spacing();
			}
			else
			{			
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(1.0f, 0.54f, 0.0f, 1.0f), "No Texture assigned");
				ImGui::Spacing();
			}

			if (ImGui::TreeNodeEx("Blending Options"))
			{
				ImGui::Text("Blending Color: ");
				if (ImGui::Combo("Source", &funcTypeSource, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0\0"))
					particleSystem->funcBlendSource = (FunctionBlendType)funcTypeSource;

				if (ImGui::Combo("Destination", &funcTypeDest, "ZERO\0ONE\0SRC_COLOR\0ONE_MINUS_SRC_COLOR\0DST_COLOR\0ONE_MINUS_DST_COLOR\0SRC_ALPHA\0ONE_MINUS_SRC_ALPHA\0DST_ALPHA\0ONE_MINUS_DST_ALPHA\0CONSTANT_COLOR\0ONE_MINUS_CONSTANT_COLOR\0CONSTANT_ALPHA\0ONE_MINUS_CONSTANT_ALPHA\0\0"))
					particleSystem->funcBlendDest = (FunctionBlendType)funcTypeDest;

				ImGui::Spacing();

				ImGui::Text("Blend Equation: ");
				if (ImGui::Combo("Equation Mode", &eqTypeSelected, "ADD\0SUBTRACT\0REVERSE SUBTRACT\0\0"))
					particleSystem->eqBlend = (EquationBlendType)eqTypeSelected;

				ImGui::TreePop();
			}
			

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNodeEx("Save & Load", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Save Particle System")) {
				App->serialization->particleCallback = this;
				App->editor->browser->OpenBrowser(BrowserState::SAVE_PARTICLE_SYSTEM); 
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Particle System")) {
				App->serialization->particleCallback = this;
				App->editor->browser->OpenBrowser(BrowserState::LOAD_PARTICLE_SYSTEM); 
			}

			ImGui::TreePop();
		}

		ImGui::Spacing;
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

	tmp_ps = name + "UUID";
	json_object_dotset_number(object, tmp_ps.c_str(), UUID);


	// ----------------------- Billboard Info ----------------------- //
	tmp_ps = name + "Billboard.Type";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->bbType);


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

	// ----------------- Particle System End Info -------------------- //

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

	// ---------------------- Emitter Info --------------------------- //
	
	// Shape
	tmp_ps = name + "Emmitter.Shape";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->emmitter.GetShape());
	// Radius
	tmp_ps = name + "Emmitter.Radius";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetRadius());
	// OutRadius
	tmp_ps = name + "Emmitter.OutRadius";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetOutRadius());
	// MaxLife
	tmp_ps = name + "Emmitter.MaxLife";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetMaxLife());
	// CurrentLife
	tmp_ps = name + "Emmitter.CurrentLife";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetCurrentLife());
	// SpawnRate
	tmp_ps = name + "Emmitter.SpawnRate";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetSpawnRate());
	// Delay
	tmp_ps = name + "Emmitter.Delay";
	json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.GetDelay());
	// Loop
	tmp_ps = name + "Emmitter.Loop";
	json_object_dotset_boolean(object, tmp_ps.c_str(), particleSystem->emmitter.GetLoop());
	// Position
	tmp_ps = name + "Emmitter.Position";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetPosition());
	// RelativePosition
	tmp_ps = name + "Emmitter.RelativePosition";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetRelativePosition());
	// WorldPosition
	tmp_ps = name + "Emmitter.WorldPosition";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetWorldPosition());
	// Rotation
	tmp_ps = name + "Emmitter.Rotation";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetRotation());
	// Scale
	tmp_ps = name + "Emmitter.Scale";
	App->fs->json_array_dotset_float3(object, tmp_ps.c_str(), particleSystem->emmitter.GetScale());
	
	// ------------------------ Burst Info --------------------------- //

	//// TimeBurst
	//tmp_ps = name + "Burst.TimeBurst";
	//json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.burst.timeToBurst);
	//// PartsInstanciate
	//tmp_ps = name + "Burst.PartsInstanciate";
	//json_object_dotset_number(object, tmp_ps.c_str(), particleSystem->emmitter.burst.partsToInstantiate);
	//// hasBursted
	//tmp_ps = name + "Burst.HasBursted";
	//json_object_dotset_boolean(object, tmp_ps.c_str(), particleSystem->emmitter.burst.hasBursted);
	//// Active
	//tmp_ps = name + "Burst.Active";
	//json_object_dotset_boolean(object, tmp_ps.c_str(), particleSystem->emmitter.burst.active);
	
	// ---------------------- Blending Info -------------------------- //
	
	// Source
	tmp_ps = name + "Blending.Source";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->funcBlendSource);
	// Destination
	tmp_ps = name + "Blending.Destination";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->funcBlendDest);
	// Equation
	tmp_ps = name + "Blending.Equation";
	json_object_dotset_number(object, tmp_ps.c_str(), (double)particleSystem->eqBlend);

	

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
		tmp_ps = name + "Resource Material Name";
		json_object_dotset_string(object, tmp_ps.c_str(), "");
		tmp_ps = name + "Resource Material UUID";
		json_object_dotset_number(object, tmp_ps.c_str(), 0);
		tmp_ps = name + "Resource Material File";
		json_object_dotset_string(object, tmp_ps.c_str(), "");
		tmp_ps = name + "Resource Material Path";
		json_object_dotset_string(object, tmp_ps.c_str(), "");
	}
}

void ComponentParticleSystem::Load(const JSON_Object * object, std::string name)
{
	std::string tmp_ps;

	// ------------------------ General Info ------------------------ //
	tmp_ps = name + "UUID";
	UUID = json_object_dotget_number(object, tmp_ps.c_str());


	// ----------------------- Billboard Info ----------------------- //
	tmp_ps = name + "Billboard.Type";
	particleSystem->bbType = (BillboardType)(int)json_object_dotget_number(object, tmp_ps.c_str());

	// --------------- Particle System Start Info -------------------- //
	// Position
	tmp_ps = name + "Start.Position";
	particleSystem->particleInfo.position = App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str());
	// Rotation
	tmp_ps = name + "Start.Rotation";
	float4 rotation = App->fs->json_array_dotget_float4_string(object, tmp_ps.c_str());
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	particleSystem->particleInfo.rotation = rot;
	// Velocity
	tmp_ps = name + "Start.Velocity";
	particleSystem->particleInfo.velocity = App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str());
	// Force
	tmp_ps = name + "Start.Force";
	particleSystem->particleInfo.force = App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str());
	// Speed
	tmp_ps = name + "Start.Speed";
	particleSystem->particleInfo.speed = json_object_dotget_number(object, tmp_ps.c_str());
	// Color
	tmp_ps = name + "Start.Color";
	particleSystem->particleInfo.color = App->fs->json_array_dotget_float4_string(object, tmp_ps.c_str());
	// Size
	tmp_ps = name + "Start.Size";
	particleSystem->particleInfo.size = json_object_dotget_number(object, tmp_ps.c_str());
	// LightColor
	tmp_ps = name + "Start.LightColor";
	particleSystem->particleInfo.lightColor = App->fs->json_array_dotget_float4_string(object, tmp_ps.c_str());
	// MaxLifeTime
	tmp_ps = name + "Start.MaxLifeTime";
	particleSystem->particleInfo.maxLifeTime = json_object_dotget_number(object, tmp_ps.c_str());
	// changeOverLifeTime
	tmp_ps = name + "Start.ChangeOverLifeTime";
	particleSystem->particleInfo.changeOverLifeTime = json_object_dotget_boolean(object, tmp_ps.c_str());
	
	
	// ----------------- Particle System End Info -------------------- //

	// Color
	tmp_ps = name + "End.Color";
	particleSystem->endInfo.color = App->fs->json_array_dotget_float4_string(object, tmp_ps.c_str());
	// Size
	tmp_ps = name + "End.Size";
	particleSystem->endInfo.size = json_object_dotget_number(object, tmp_ps.c_str());
	// LightColor
	tmp_ps = name + "End.LightColor";
	particleSystem->endInfo.lightColor = App->fs->json_array_dotget_float4_string(object, tmp_ps.c_str());
	// Force
	tmp_ps = name + "End.Force";
	particleSystem->endInfo.force = App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str());

	// ---------------------- Emitter Info --------------------------- //

	// Shape
	tmp_ps = name + "Emmitter.Shape";
	particleSystem->emmitter.SetShape((Emmitter_Shape)(int)json_object_dotget_number(object, tmp_ps.c_str()));
	// Radius
	tmp_ps = name + "Emmitter.Radius";
	particleSystem->emmitter.SetRadius(json_object_dotget_number(object, tmp_ps.c_str()));
	// OutRadius
	tmp_ps = name + "Emmitter.OutRadius";
	particleSystem->emmitter.SetOutRadius(json_object_dotget_number(object, tmp_ps.c_str()));
	// MaxLife
	tmp_ps = name + "Emmitter.MaxLife";
	particleSystem->emmitter.SetMaxLife(json_object_dotget_number(object, tmp_ps.c_str()));
	// CurrentLife
	tmp_ps = name + "Emmitter.CurrentLife";
	particleSystem->emmitter.SetCurrentLife(json_object_dotget_number(object, tmp_ps.c_str()));
	// SpawnRate
	tmp_ps = name + "Emmitter.SpawnRate";
	particleSystem->emmitter.SetSpawnRate(json_object_dotget_number(object, tmp_ps.c_str()));
	// Delay
	tmp_ps = name + "Emmitter.Delay";
	particleSystem->emmitter.SetDelay(json_object_dotget_number(object, tmp_ps.c_str()));
	// Loop
	tmp_ps = name + "Emmitter.Loop";
	particleSystem->emmitter.SetLoop(json_object_dotget_boolean(object, tmp_ps.c_str()));
	// Position
	tmp_ps = name + "Emmitter.Position";
	particleSystem->emmitter.SetPosition(App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str()));
	// RelativePosition
	tmp_ps = name + "Emmitter.RelativePosition";
	particleSystem->emmitter.SetRelativePosition(App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str()));

	// WorldPosition
	//tmp_ps = name + "Emmiter.WorldPosition";
	//particleSystem->emmitter.SetWoldPosition(App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str()));

	// Rotation
	tmp_ps = name + "Emmitter.Rotation";
	particleSystem->emmitter.SetRotation(App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str()));
	// Scale
	tmp_ps = name + "Emmitter.Scale";
	particleSystem->emmitter.SetScale(App->fs->json_array_dotget_float3_string(object, tmp_ps.c_str()));
	
	
	// ------------------------ Burst Info --------------------------- //

	//// TimeBurst
	//tmp_ps = name + "Burst.TimeBurst";
	//particleSystem->emmitter.burst.timeToBurst = json_object_dotget_number(object, tmp_ps.c_str());
	//// PartsInstanciate
	//tmp_ps = name + "Burst.PartsInstanciate";
	//particleSystem->emmitter.burst.partsToInstantiate = json_object_dotget_number(object, tmp_ps.c_str());
	//// hasBursted
	//tmp_ps = name + "Burst.HasBursted";
	//particleSystem->emmitter.burst.hasBursted = json_object_dotget_boolean(object, tmp_ps.c_str());
	//// Active
	//tmp_ps = name + "Burst.Active";
	//particleSystem->emmitter.burst.active = json_object_dotget_boolean(object, tmp_ps.c_str());


	// ---------------------- Blending Info -------------------------- //
	// Source
	tmp_ps = name + "Blending.Source";
	particleSystem->funcBlendSource = (FunctionBlendType)(int)json_object_dotget_number(object, tmp_ps.c_str());
	// Destination
	tmp_ps = name + "Blending.Destination";
	particleSystem->funcBlendDest = (FunctionBlendType)(int)json_object_dotget_number(object, tmp_ps.c_str());
	// Equation
	tmp_ps = name + "Blending.Equation";
	particleSystem->eqBlend = (EquationBlendType)(int)json_object_dotget_number(object, tmp_ps.c_str());

	// ------------------- Material Resource Info -------------------- //
	tmp_ps = name + "Resource Material UUID";
	UID resUUID = json_object_dotget_number(object, tmp_ps.c_str());

	tmp_ps = name + "Resource Material Path";
	std::string exported_file = json_object_dotget_string(object, tmp_ps.c_str());

	tmp_ps = name + "Resource Material File";
	std::string file = json_object_dotget_string(object, tmp_ps.c_str());

	tmp_ps = name + "Resource Material Name";
	std::string name_file = json_object_dotget_string(object, tmp_ps.c_str());

	if (resUUID > 0)
	{
		matTypeSelected = 1;
		App->editor->textureBrowser->callback = this;
		ResourceTexture* resMat = (ResourceTexture*)App->resources->GetResource(resUUID);
		if (resMat != nullptr)
		{
			this->AssignResource(resUUID);
			this->particleSystem->resMat = resMat;
			resMat->LoadToMemory();
		}
		else
		{
			resMat = (ResourceTexture*)App->resources->CreateNewResource(Resource::R_TEXTURE, resUUID, name_file.c_str(), file.c_str(), exported_file.c_str(), true);
			this->AssignResource(resUUID);
			this->particleSystem->resMat = resMat;
			resMat->LoadToMemory();


		}
	}
	else {
		matTypeSelected = 0;
//		App->editor->textureBrowser->callback->AssignResource(0);
		currentResource = nullptr;
		//particleSystem->resMat = nullptr;
		//App->editor->textureBrowser->callback = nullptr;
	}
}

