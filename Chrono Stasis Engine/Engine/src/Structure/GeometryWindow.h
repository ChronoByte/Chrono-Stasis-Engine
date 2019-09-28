#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

#include "MathGeoLib/include/MathGeoLib.h"

class Application;

class GeometryWindow : public Window
{

public:

	GeometryWindow(bool startOpened = false);
	~GeometryWindow();

	void Draw() override;
	void MathTest();

private:

	Sphere sphere_1;
	Sphere sphere_2;

	Capsule capsule_1;
	Capsule capsule_2;

	Triangle triangle_1;
	Triangle triangle_2;

	Plane plane_1;
	Plane plane_2;

	AABB aabb_1;
	AABB aabb_2;

	Ray ray_1;

	OBB obb_1;
	OBB obb_2;
};
