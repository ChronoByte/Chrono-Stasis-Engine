#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

#include <list>
#include <vector>

class Application;

class ConsoleWindow : public Window
{

public:

	ConsoleWindow(Application* app);
	~ConsoleWindow();

	void Draw() override;


	void PushLog(char* log);
	void PrintAllLogs();
	void Clear();

private: 

	bool scrolltobottom = false; 
	std::vector<char*> loglist; 
};
