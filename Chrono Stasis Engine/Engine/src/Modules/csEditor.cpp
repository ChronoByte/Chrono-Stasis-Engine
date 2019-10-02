#include "csGlobals.h"
#include "csApp.h"
#include "csEditor.h"
#include <string>
#include "time.h"
#include "csInput.h"

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
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debugMode = !debugMode; 
	

	if (debugMode)
		DrawAxis();

	update_status ret = UPDATE_CONTINUE; 

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
			if (ImGui::MenuItem("Collision Tester", "", geometryWin->GetBool())) {}
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
			SetHelpMenu();
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

	if (random_panel) 
		DrawRandomPanel();
	

	glLineWidth(1.0f);

	if(!glIsEnabled(GL_LIGHTING))
		glColor3f(255, 255, 255);

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


	DrawCubeVertexArray();

	return ret;
}

void ModuleEditor::SetHelpMenu()
{
	if (ImGui::MenuItem("Documentation"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/wiki");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("Download Latest Version"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/releases");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("Report a bug"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/issues");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("About"))
		about->SwitchActive();
}

void ModuleEditor::DrawRandomPanel()
{
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
		rand3 = pcg32_boundedrand_r(&rng_bounded2, (max - min) + 1);

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

void ModuleEditor::DrawAxis()
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glLineWidth(1.0f);
}


void ModuleEditor::DrawCubeDirectMode()
{
	glBegin(GL_TRIANGLES);
	glColor4f(255, 0, 128, 255);

	// Face 1
	glVertex3f(0.f, 0.f, 0.f);	
	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);

	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 1.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);


	// Face 2
	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(1.f, 0.f, 1.f);
	glVertex3f(0.f, 1.f, 1.f);

	glVertex3f(1.f, 0.f, 1.f);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(0.f, 1.f, 1.f);


	// Face 3
	glVertex3f(1.f, 0.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	glVertex3f(1.f, 0.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);


	// Face 4
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);

	// Face 5
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);


	// Face 6
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 1.f);

	glVertex3f(1.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);

	glEnd();
}

void ModuleEditor::DrawCubeVertexArray()
{
	/*float vertices[8 * 3] =
	{
		0.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 1.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f
	};*/

	float vertices[6 * 9 * 2] = // Size its = 6 faces * 9 vertex * 2 triangles 
	{
		// Face 1
		0.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		0.f, 1.f, 0.f,

		0.f, 0.f, 1.f,
		0.f, 1.f, 1.f,
		0.f, 1.f, 0.f,

		// Face 2
		0.f, 0.f, 1.f,
		1.f, 0.f, 1.f,
		0.f, 1.f, 1.f,

		1.f, 0.f, 1.f,
		1.f, 1.f, 1.f,
		0.f, 1.f, 1.f,

		// Face 3
		1.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,

		1.f, 0.f, 1.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f,


		// Face 4
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 1.f, 0.f,

		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f,


		// Face 5
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		1.f, 1.f, 0.f,

		1.f, 1.f, 1.f,
		1.f, 1.f, 0.f,
		0.f, 1.f, 1.f,


		// Face 6
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		0.f, 0.f, 1.f,

		1.f, 0.f, 1.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f
	};



	GLuint myId = 0;
	glGenBuffers(1, &myId);	glBindBuffer(GL_ARRAY_BUFFER, myId);	glColor3f(255, 0, 158);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 9 * 2, vertices, GL_STATIC_DRAW);		// 6 faces * 9 vertex * 2 triangles 

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, (6 * 9 * 2) / 3); // 6 faces * 9 vertex * 2 triangles / 3 vertex per trianble
	glDisableClientState(GL_VERTEX_ARRAY);
}

//void ModuleEditor::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

