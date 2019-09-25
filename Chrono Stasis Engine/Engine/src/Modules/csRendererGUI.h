#pragma once
#include "csModule.h"
#include "csGlobals.h"


struct ColorStyles
{
	const char* ORANGE = "ORANGE";
	const char* BLACK = "BLACK";
};

class ModuleUI : public Module
{
public:
	
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Init() override;
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void StyleLoader(const char* name);
	
public:
	ColorStyles color;
};