#pragma once

#include "csGlobals.h"
class Application;

class Window
{

public:

	Window(Application* parent) : App(parent) {}

	~Window() { App = nullptr; }


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

	virtual void Draw() {}

	virtual bool CleanUp()
	{
		return true;
	}

public:

	void Activate()
	{
		active = true; 
	}

	void Deactivate()
	{
		active = false; 
	}

	bool GetActive()
	{
		return active;
	}

	void SwitchActive()
	{
		active = !active; 
	}

protected:

	bool active = false; 

protected:
	Application* App = nullptr;
};
