#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"

class Application;
class GameObject; 

class HierarchyWindow : public Window
{

public:

	HierarchyWindow(bool startOpened = false);
	~HierarchyWindow();

	void Draw() override;
	void CreateRecursiveTreeNodes(const GameObject* parent); 

private:
	const GameObject* selectedGo = nullptr; 
	//friend class InspectorWindow; 

};

