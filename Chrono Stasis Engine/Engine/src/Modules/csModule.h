#pragma once

#include "JSON/parson.h"
#include <string>

class Application;

class Module
{
public:
	std::string name;

private:
	bool enabled;

public:

	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* node)
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

	bool IsEnabled() const { return enabled; }

	void Enable() 
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false; 
			CleanUp(); 
		}
	}
};