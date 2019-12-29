#include "ParticlesWindow.h"
#include "csApp.h"
#include "csScene.h"
#include "ComponentParticleSystem.h"
#include "Particle_System/csParticleSystem.h"

ParticlesWindow::ParticlesWindow(bool startOpened) : Window(startOpened)
{
}

ParticlesWindow::~ParticlesWindow()
{
}

void ParticlesWindow::Draw()
{
	GameObject* selected = App->scene->GetSelected();
	if (selected == nullptr || !selected->HasComponent(ComponentType::C_PARTICLE_SYSTEM))
		return;

	ComponentParticleSystem* particleSystem = (ComponentParticleSystem*)selected->FindComponent(ComponentType::C_PARTICLE_SYSTEM);
	owner = particleSystem->GetSystem();

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Particle System", nullptr, flags))
	{
		/*ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.37f,0.0f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.95f,0.5f,0.0f,0.7f });*/

		if(ImGui::Button("Restart System")) { owner->ResetSystem(); }

		ImGui::Text("Playback Time: ");	ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.3f", owner->emmitter.GetCurrentLife());
		ImGui::Text("Particles: ");	ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", owner->GetTotalParticles());

	}
	ImGui::End();

}

ParticleSystem * ParticlesWindow::GetSystem() const
{
	return owner;
}

void ParticlesWindow::SetSystem(ParticleSystem * system)
{
	owner = system;
}
