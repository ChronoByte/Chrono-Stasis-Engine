#include "GeometryWindow.h"
#include "csApp.h"
#include "csWindow.h"

GeometryWindow::GeometryWindow(bool startOpened) : Window(startOpened)
{	
	sphere_1 = Sphere({ 0.0f,0.0f,0.0f }, 1.0f);
	sphere_2 = Sphere({ 0.0f,0.0f,0.0f }, 1.0f);

	capsule_1 = Capsule(float3(3.f, 0.f, 0.f), float3(3.f, 2.f, 0.f), 1.f);
	capsule_2 = Capsule(float3(0.f, 0.f, 0.f), float3(0.f, 2.f, 0.f), 1.f);

	triangle_1 = Triangle(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0f, 0.0f), float3(0.5f, 1.0f, 0.0f));
	triangle_2 = Triangle(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), float3(0.0f, 1.0f, 0.5f));

	plane_1 = Plane(float3(0.f, 0.f, 1.f), 0.0f);
	plane_2 = Plane(float3(0.f, 0.f, 1.f), 2.0f);

	ray_1 = Ray(float3(5.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f));

	obb_1 = OBB(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f), float3(1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f));
	obb_2 = OBB(float3(2.5f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f), float3(1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f));
}


GeometryWindow::~GeometryWindow() {}


void GeometryWindow::Draw()
{
	MathTest();
}

void GeometryWindow::MathTest()
{
	ImGui::Begin("MathGeoLib Collision Tester", &active, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Creation"))
	{
		static float pos[3] = { 0,0,0 };
		static float size[3] = { 1,1,1 };

		ImGui::SliderFloat3("Position", pos, -10, 10);
		ImGui::SliderFloat3("Size", size, 0, 10);
		
		if (ImGui::Button("Create Cube"))
			App->editor->CreateCube(vec3(pos[0], pos[1], pos[2]), size[0], size[1], size[2]);
	}


	if (ImGui::CollapsingHeader("Spheres"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1: ", &sphere_1.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1: ", &sphere_1.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &sphere_1.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 1: ", &sphere_1.r, -5.0f, 5.0f);

		ImGui::Text("Sphere 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2: ", &sphere_2.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2: ", &sphere_2.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 3: ", &sphere_2.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 2: ", &sphere_2.r, -5.0f, 5.0f);

		if (sphere_1.Contains(sphere_2))
			ImGui::Text("Overlaping!");
		else
			ImGui::Text("Not Overlaping!");

	}
	if (ImGui::CollapsingHeader("AABB"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1: ", &sphere_1.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1: ", &sphere_1.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &sphere_1.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 1: ", &sphere_1.r, -5.0f, 5.0f);

		ImGui::Text("Sphere 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2: ", &sphere_2.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2: ", &sphere_2.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 3: ", &sphere_2.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 2: ", &sphere_2.r, -5.0f, 5.0f);

		aabb_1 = sphere_1.MinimalEnclosingAABB();
		aabb_2 = sphere_2.MinimalEnclosingAABB();

		if (aabb_1.Intersects(aabb_2))
			ImGui::Text("Intersect!");
		else
			ImGui::Text("Don't intersect!");
	}


	if (ImGui::CollapsingHeader("Capsules"))
	{

		ImGui::Text("Capsule 1:");
		ImGui::SameLine();

		ImGui::SliderFloat("C_Radius 1:", &capsule_1.r, 0.0f, 5.0f);

		ImGui::Text("Capsule 2:");
		ImGui::SameLine();

		ImGui::SliderFloat("C_Radius 2: ", &capsule_2.r, 0.0f, 5.0f);


		if (capsule_1.Intersects(capsule_2))
			ImGui::Text("Intersect!");
		else
			ImGui::Text("Don't intersect!");

	}
	if (ImGui::CollapsingHeader("Triangles"))
	{

		ImGui::Text("Triangle 1 All vertex:");
		ImGui::SameLine();
		ImGui::SliderFloat3("T X 1", (float*)&triangle_1.a, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat3("T Y 1", (float*)&triangle_1.b, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat3("T Z 1", (float*)&triangle_1.c, -5.0f, 5.0f);

		ImGui::Text("Triangle 2 All vertex:");
		ImGui::SameLine();
		ImGui::SliderFloat3("T X 2", (float*)&triangle_2.a, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat3("T Y 2", (float*)&triangle_2.b, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat3("T Z 2", (float*)&triangle_2.c, -5.0f, 5.0f);



		if (triangle_1.Intersects(triangle_2))
			ImGui::Text("Intersect!");
		else
			ImGui::Text("Don't intersect!");
	}
	if (ImGui::CollapsingHeader("Planes"))
	{

		ImGui::Text("Plane 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1", &plane_1.normal.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &plane_1.normal.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &plane_1.normal.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Offset from Origin 1", &plane_1.d, 0.0f, 5.0f);

		ImGui::Text("Plane 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &plane_2.normal.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &plane_2.normal.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &plane_2.normal.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Offset from Origin 2", &plane_2.d, 0.0f, 5.0f);

		if (plane_1.Intersects(plane_2))
			ImGui::Text("Intersect!");
		else
			ImGui::Text("Don't intersect!");
	}

	if (ImGui::CollapsingHeader("Rays"))
	{
		ImGui::Text("Ray 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X dir", &ray_1.dir.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y dir", &ray_1.dir.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z dir", &ray_1.dir.z, -5.0f, 5.0f);

		if (ray_1.Intersects(sphere_1))
			ImGui::Text("Sphere Intersection");
		else
			ImGui::Text("Not Sphere Intersection");
	}

	if (ImGui::CollapsingHeader("OBB"))
	{

		ImGui::Text("OBB 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1", &obb_1.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &obb_1.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &obb_1.pos.z, -5.0f, 5.0f);


		ImGui::Text("OBB 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &obb_2.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &obb_2.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &obb_2.pos.z, -5.0f, 5.0f);

		if (obb_1.Intersects(obb_2))
			ImGui::Text("Intersect!");
		else
			ImGui::Text("Don't intersect!");
	}
	ImGui::End();
}