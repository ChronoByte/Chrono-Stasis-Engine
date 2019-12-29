#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"

class ParticleSystem;

class ParticlesWindow : public Window
{

public:

	ParticlesWindow(bool startOpened = false);
	~ParticlesWindow();

	void Draw() override;

	ParticleSystem* GetSystem() const;
	void SetSystem(ParticleSystem* system);

private: 

	ParticleSystem* owner = nullptr; 
};


