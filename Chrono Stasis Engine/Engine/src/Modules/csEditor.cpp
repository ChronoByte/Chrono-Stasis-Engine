#include "csGlobals.h"
#include "csApp.h"
#include "csEditor.h"
#include <string>
#include "time.h"


// Including windows
#include "src/Structure/ConfigWindow.h"
#include "src/Structure/GeometryWindow.h"
#include "src/Structure/AboutWindow.h"
#include "src/Structure/ConsoleWindow.h"

ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	config = new ConfigWindow();
	geometryWin = new GeometryWindow();
	about = new AboutWindow();	
	console = new ConsoleWindow(true);
}

ModuleEditor::~ModuleEditor()
{
}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	config->Start();

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

	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	delete config; 
	config = nullptr; 

	delete geometryWin;
	geometryWin = nullptr; 

	delete about;
	about = nullptr; 

	delete console; 
	console = nullptr; 

	return true;
}

void ModuleEditor::Log(char * log) const
{
	if (console != nullptr)
		console->PushLog(log);
}

// Update
update_status ModuleEditor::Update(float dt)
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
			if (ImGui::MenuItem("Demo Window", "",  &show_demo_window))	{}
			if (ImGui::MenuItem("Console", "", console->GetBool()))	{}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Style"))
		{
			if (ImGui::MenuItem("Orange")) { App->gui->StyleLoader(App->gui->color.ORANGE);  }
			if (ImGui::MenuItem("Black")) { App->gui->StyleLoader(App->gui->color.BLACK); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GeoMath"))
		{
			if (ImGui::MenuItem("Collision Tester"))
			{
				geometryWin->SwitchActive();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Random (PCG)"))
		{
			if (ImGui::MenuItem("Test pcg", "", &random_panel)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Configuration"))
		{
			config->Activate();
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/wiki");

			if (ImGui::MenuItem("Download Latest Version"))
				App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/releases");

			if (ImGui::MenuItem("Report a bug"))
				App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/issues");

			if (ImGui::MenuItem("About"))
				about->SwitchActive();

			ImGui::EndMenu(); 
		}

		ImGui::EndMainMenuBar();
	}

	if (config->GetActive())
		config->Draw();

	if (geometryWin->GetActive())
		geometryWin->Draw();

	if (about->GetActive())
		about->Draw(); 

	if (console->GetActive())
		console->Draw(); 

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

//void ModuleEditor::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

