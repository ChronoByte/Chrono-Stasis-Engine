#include "ComponentParticleSystem.h"
#include "csApp.h"
#include "Particle_System/csParticleSystem.h"

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
	particleSystem->Update(dt);
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

	if (ImGui::CollapsingHeader("Particle System", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active System", &active); // Can't repeat checkbox name (!!)

		if (ImGui::TreeNode("Emmitter"))
		{
			ParticleEmmitter* emmitter = &particleSystem->emmitter; 

			ImGui::Checkbox("Draw Emmitter", &drawEmmitter);

			bool loop = emmitter->GetLoop(); 
			if(ImGui::Checkbox("Loop", &loop)) { emmitter->SetLoop(loop); }

			if (ImGui::Combo("Shape", &shapeSelected, "Sphere\0Hemisphere\0Cube\0Cone\0Plane\0\0"))	{ emmitter->SetShape((Emmitter_Shape)shapeSelected); }

			int spawnRate = emmitter->GetSpawnRate();
			if (ImGui::InputInt("Spawn Rate", &spawnRate)) { emmitter->SetSpawnRate(spawnRate); }

			float3 pos = particleSystem->emmitter.GetPosition(); 
			if (ImGui::DragFloat3("Position", (float*)&pos)) { emmitter->SetPosition(pos); }

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Particle"))
		{
			// Initial State || Final State
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Renderer"))
		{
			// Billboarding ?
			ImGui::TreePop();
		}
	}
}

void ComponentParticleSystem::Save(JSON_Object * object, std::string name, bool saveScene, uint & countResources) const
{

}

void ComponentParticleSystem::Load(const JSON_Object * object, std::string name)
{

}

