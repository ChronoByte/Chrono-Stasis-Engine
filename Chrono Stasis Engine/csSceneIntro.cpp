#include "csGlobals.h"
#include "csApp.h"
#include "csSceneIntro.h"
//#include "Primitive.h"
//#include "PhysBody3D.h"
#include <string>
#include "time.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#include "MathGeoLib/include/Geometry/Sphere.h"

// TODO: Differentiate between debug mode and release mode
#pragma comment (lib, "MathGeoLib/libx86/Debug_Lib/MathGeoLib.lib")



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	float3 vector(0.0f, 0.0f, 1.0f);
	Plane* plane = new Plane(vector, 1.5f);
	
	/*Plane plane2(vector, 1.5f);*/
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
			if (ImGui::MenuItem("Test pcg", "", &random_panel)) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Style"))
		{
			if (ImGui::MenuItem("Orange")) { App->ui->StyleLoader(App->ui->color.ORANGE);  }
			if (ImGui::MenuItem("Black")) { App->ui->StyleLoader(App->ui->color.BLACK); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

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

