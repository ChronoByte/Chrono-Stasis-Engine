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
class GeometryWindow; 
class AboutWindow;
class ConsoleWindow; 
class RandomWindow; 

struct shapeInfo {
	uint myId = 0; 
	uint indexId = 0; 
	par_shapes_mesh* myMesh = nullptr; 
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
	void DrawShapes(); 

	void DrawAxis();

	void Log(char* log) const; 
	
	//void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	ConfigWindow*	config = nullptr;
	GeometryWindow* geometryWin = nullptr; 
	AboutWindow*	about = nullptr;
	PhysBody3D*		sensor = nullptr;
	ConsoleWindow*	console = nullptr; 
	RandomWindow*	randomWin = nullptr; 
	
	bool show_demo_window = false;
	bool debugMode = true; 

	std::list<shapeInfo*> shapes; 

	
};
