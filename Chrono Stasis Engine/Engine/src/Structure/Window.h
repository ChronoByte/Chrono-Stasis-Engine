#pragma once

#include "csGlobals.h"
class Application;

class Window
{

public:

	Window(Application* parent) : App(parent) {}

	~Window() { delete App;  }


	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

protected:
	Application* App = nullptr;
};
