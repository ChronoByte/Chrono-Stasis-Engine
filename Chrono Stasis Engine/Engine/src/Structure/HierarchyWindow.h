#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class Application;
class GameObject; 

class HierarchyWindow : public Window
{

public:

	HierarchyWindow(bool startOpened = false);
	~HierarchyWindow();

	void Draw() override;
	void CreateRecursiveTreeNodes(GameObject* parent); 

};

