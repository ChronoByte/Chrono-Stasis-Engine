#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class Application;

class SceneViewWindow : public Window
{

public:

	SceneViewWindow(bool startOpened = false);
	~SceneViewWindow();

	void Draw() override;
	void UpdateGuizmo(); 

	float GetWindowWidth() const;
	float GetWindowHeight() const;

	float GetMouseXInWindow() const; 
	float GetMouseYInWindow() const; 

public:
	
	float width = 0.f;
	float height = 0.f;

	float regionX = 0.f; 
	float regionY = 0.f; 

	float mouseX = 0.f;
	float mouseY = 0.f;

	float windowPosX = 0.f; 
	float windowPosY = 0.f;

	bool zBuffer = false; 
};


