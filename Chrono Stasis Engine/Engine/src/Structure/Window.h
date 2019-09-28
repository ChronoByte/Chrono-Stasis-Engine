#pragma once

#include "csGlobals.h"
class Application;

class Window
{

protected:

	bool active = false;

public:

	Window(bool startOpened = false) : active(startOpened) {}

	~Window() {}


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

	bool GetActive() const
	{
		return active;
	}

	void SwitchActive()
	{
		active = !active; 
	}

	void SetActive(bool set)
	{
		active = set; 
	}

	bool* GetBool()
	{
		return &active;
	}

};
