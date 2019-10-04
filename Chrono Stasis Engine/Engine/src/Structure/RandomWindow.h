#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

#include "imgui/imgui.h"
#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"

class Application;

class RandomWindow : public Window
{

public:

	RandomWindow(bool startOpened = false);
	~RandomWindow();

	void Draw() override;
	void RandomTest();

private:

	double rand1 = 0;
	uint32_t rand2 = 0;
	uint32_t rand3 = 0;

	uint32_t nums[6];
	uint64_t seeds[2];

	int min = 0;
	int	max = 0;

	//Struct with state + inc
	pcg32_random_t rng; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b
	pcg_state_setseq_64 rng_bounded2;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
};
