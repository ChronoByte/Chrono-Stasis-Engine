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
	void CreateRecursiveTreeNodes(GameObject* parent); 

	GameObject* GetSelected() const;

private:
	GameObject* selectedGo = nullptr;
	//friend class InspectorWindow; 

};

