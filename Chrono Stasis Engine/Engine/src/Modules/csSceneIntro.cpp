#include "csGlobals.h"
#include "csApp.h"
#include "csSceneIntro.h"
//#include "Primitive.h"
//#include "PhysBody3D.h"
#include <string>
#include "time.h"

//#include "MathGeoLib/include/MathGeoLib.h"
//#include "MathGeoLib/include/MathBuildConfig.h"
//#include "MathGeoLib/include/MathGeoLibFwd.h"

//#include "MathGeoLib/include/Geometry/Sphere.h"

// TODO: Differentiate between debug mode and release mode
//#pragma comment (lib, "DependenciesMathGeoLib/libx86/Debug_Lib/MathGeoLib.lib")

#include "src/Structure/ConfigWindow.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	config = new ConfigWindow(app);
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	


	//Seeds random number generator
	//pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	
	//Seed with external entropy
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);

	//Adding flags using bit-wise OR
	window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize;

	rand1 = 0;
	rand2 = 0;
	rand3 = 0;

	
	sphere_1 = Sphere({0.0f,0.0f,0.0f}, 1.0f);
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


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{


	update_status ret = UPDATE_CONTINUE; 

	Sphere s({ 0, 0, 0 }, 5);
	Sphere s2({ 0, 5,0 }, 5);

	if (s.Intersects(s2))
	{
		LOG("There was an intersection");
	}







	//d = ldexp(pcg32_random_r(&rng), -32); //generating a flaoting points between [0,1) rounded nearest multiple of 1/2^32

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close", "Ctrl+W")) { App->to_close_app = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo Window", "",  &show_demo_window)) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Style"))
		{
			if (ImGui::MenuItem("Orange")) { App->ui->StyleLoader(App->ui->color.ORANGE);  }
			if (ImGui::MenuItem("Black")) { App->ui->StyleLoader(App->ui->color.BLACK); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GeoMath"))
		{
			if (ImGui::MenuItem("Collision Tester","", &math_test)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Random (PCG)"))
		{
			if (ImGui::MenuItem("Test pcg", "", &random_panel)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Configuration"))
		{
			config->Active();
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (config->GetActive())
		config->Draw();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (random_panel) {
		ImGui::SetNextWindowSize({ 300,500 });

		ImGui::Begin("Random Generator", &random_panel, window_flags);
		ImGui::Separator();
		ImGui::Text("Randoms rounded [0,1)");
		ImGui::Separator();
		
		if (ImGui::Button("Generate 1"))
			rand1 = ldexp(pcg32_random_r(&rng), -32);
		
		ImGui::SameLine();
		ImGui::Text(std::to_string(rand1).c_str());

		ImGui::Separator();
		ImGui::Text("Randoms rounded [0,6)");

		if (ImGui::Button("Generate 2"))
			rand2 = pcg32_boundedrand_r(&rng_bounded, 6);

		ImGui::SameLine();
		ImGui::Text("%i", (int)rand2);

		ImGui::Separator();

		ImGui::Text("Randoms rounded to anyone [min/max]");
		
		ImGui::InputInt("Max", &max);
		ImGui::InputInt("Min", &min);

		if (ImGui::Button("Generate 3") && max >= min)
			rand3 = pcg32_boundedrand_r(&rng_bounded2, (max - min)+1);

		ImGui::SameLine();
		ImGui::Text("%i", (int)(rand3 + min));

		ImGui::Separator();

		ImGui::Text(" Make some 32-bit numbers ");
		
		if (ImGui::Button("Generate 4")) {
			for (int i = 0; i < 6; ++i)
				nums[i] = pcg32_random_r(&rng);
		}

		for (int i = 0; i < 6; ++i) {
			ImGui::Text("32bit: ");
			ImGui::SameLine();
			ImGui::Text("0x%08x", (int)nums[i]);
		}


		ImGui::End();
	}

	if (math_test)
	{
		ImGui::Begin("MathGeoLib Collision Tester", &math_test, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();

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

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	return ret;
}

//void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

