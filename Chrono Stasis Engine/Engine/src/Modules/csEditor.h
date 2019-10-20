#pragma once
#include "csModule.h"
#include "csGlobals.h"

#include "glew/include/GL/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#include "par/par_shapes.h"
#include "glm/glmath.h"

#include <list>
#define MAX_SNAKE 2

struct PhysBody3D;
class ConfigWindow;
class CollisionWindow; 
class AboutWindow;
class ConsoleWindow; 
class RandomWindow; 
class InspectorWindow;
class HierarchyWindow; 
class GeometryWindow; 

class Window;

enum WindowType
{
	INSPECTOR,
	CONFIGURATION,
	ABOUT,
	HIERARCHY,
	SCENEWORLD
};

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SetHelpMenu();

	void CreateCube(const vec3& position, const uint& length = 1u, const uint& width = 1u, const uint& height = 1u);

	void Log(char* log) const; 
	
	//void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	ConfigWindow*		config = nullptr;
	CollisionWindow*	collisionWin = nullptr; 
	AboutWindow*		about = nullptr;
	ConsoleWindow*		console = nullptr; 
	RandomWindow*		randomWin = nullptr; 
	InspectorWindow*	inspector = nullptr;
	HierarchyWindow*	hierarchy = nullptr; 
	GeometryWindow*		geometryWin = nullptr; 

	bool show_demo_window = false;

	std::vector<Window*> windowManager;
};
