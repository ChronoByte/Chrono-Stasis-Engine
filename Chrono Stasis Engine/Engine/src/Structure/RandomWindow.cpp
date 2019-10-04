#include "RandomWindow.h"
#include "csApp.h"
#include "csWindow.h"

RandomWindow::RandomWindow(bool startOpened) : Window(startOpened)
{	
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);

	//Seeds random number generator
	//pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);

	//Seed with external entropy
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);

	window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize;

}


RandomWindow::~RandomWindow() {}


void RandomWindow::Draw()
{
	RandomTest(); 
}

void RandomWindow::RandomTest()
{
	ImGui::SetNextWindowSize({ 300,500 });

	ImGui::Begin("Random Generator", &active, window_flags);
	ImGui::Separator();
	ImGui::Text("Randoms rounded [0,1)");
	ImGui::Separator();

	if (ImGui::Button("Generate 1"))
		rand1 = ldexp(pcg32_random_r(&rng), -32);

	ImGui::SameLine();
	ImGui::Text(std::to_string(rand1).c_str());

	ImGui::Separator();
	ImGui::Text("Randoms rounded [0,6)");

	if (ImGui::Button("Generate 2"))
		rand2 = pcg32_boundedrand_r(&rng_bounded, 6);

	ImGui::SameLine();
	ImGui::Text("%i", (int)rand2);

	ImGui::Separator();

	ImGui::Text("Randoms rounded to anyone [min/max]");

	ImGui::InputInt("Max", &max);
	ImGui::InputInt("Min", &min);

	if (ImGui::Button("Generate 3") && max >= min)
		rand3 = pcg32_boundedrand_r(&rng_bounded2, (max - min) + 1);

	ImGui::SameLine();
	ImGui::Text("%i", (int)(rand3 + min));

	ImGui::Separator();

	ImGui::Text(" Make some 32-bit numbers ");

	if (ImGui::Button("Generate 4")) {
		for (int i = 0; i < 6; ++i)
			nums[i] = pcg32_random_r(&rng);
	}

	for (int i = 0; i < 6; ++i) {
		ImGui::Text("32bit: ");
		ImGui::SameLine();
		ImGui::Text("0x%08x", (int)nums[i]);
	}


	ImGui::End();
}

