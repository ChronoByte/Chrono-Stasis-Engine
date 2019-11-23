#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class TimeWindow : public Window
{

public:

	TimeWindow(bool startOpened = false);
	~TimeWindow();

	void Draw() override;

	void ShowOptions();
	
	void DisplayOnStopped();
	
	void PlayButton();
	void StepButton();
	void TryToOpenGameWindow();
	void PauseButton();
	void DisplayOnPlay();
	void StopButton();
	void ResumeButton();

private: 
	bool showOptions = false; 
	bool wasGameOpen = false; 

};


