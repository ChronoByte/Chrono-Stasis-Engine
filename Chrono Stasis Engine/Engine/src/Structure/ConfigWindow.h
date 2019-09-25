#pragma once

#include "csGlobals.h"

class ConfigWindow
{

public:

	ConfigWindow();
	~ConfigWindow();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:



};
