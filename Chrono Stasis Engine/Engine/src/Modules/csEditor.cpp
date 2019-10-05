#include "csGlobals.h"
#include "csApp.h"
#include "csEditor.h"
#include <string>
#include "time.h"
#include "csInput.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"

// Including windows
#include "src/Structure/ConfigWindow.h"
#include "src/Structure/GeometryWindow.h"
#include "src/Structure/AboutWindow.h"
#include "src/Structure/ConsoleWindow.h"
#include "src/Structure/RandomWindow.h"

ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	name = "Editor";

	config = new ConfigWindow();
	geometryWin = new GeometryWindow();
	about = new AboutWindow();	
	console = new ConsoleWindow(true);
	randomWin = new RandomWindow(); 

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
	
	std::list<shapeInfo*>::const_iterator item = shapes.begin();

	for (item; item != shapes.end(); item++)
	{
		par_shapes_free_mesh((*item)->myMesh); 
	}

	shapes.clear(); 

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

	if (ImGui::Begin("Dock Demo"))
	{
		// dock layout by hard-coded or .ini file
		ImGui::BeginDockspace();

		if (ImGui::BeginDock("Dock 1")) {
			ImGui::Text("Sebas");
		}
		ImGui::EndDock();

		if (ImGui::BeginDock("Dock 2")) {
			ImGui::Text("Carlos");
		}
		ImGui::EndDock();

		if (ImGui::BeginDock("Dock 3")) {
			ImGui::Text("E.T.");
		}
		ImGui::EndDock();
		if (ImGui::BeginDock("Configuration Window")) {
			config->DockDraw();
		}
		ImGui::EndDock();

		ImGui::EndDockspace();
	}
	ImGui::End();

	
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

		if (ImGui::BeginMenu("Configuration"))
		{
			config->Activate();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Collision Tester", "", geometryWin->GetBool())) {}
			if (ImGui::MenuItem("Random Tester", "", randomWin->GetBool())) {}

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

	if (randomWin->GetActive())
		randomWin->Draw();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	glLineWidth(1.0f);

	glColor4f(1.0f, 1.0f ,1.0f, 1.0f);

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

	DrawShapes();

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

void ModuleEditor::CreateCube(const vec3& position, const uint & length, const uint & width, const uint & height)
{
	par_shapes_mesh* mesh = nullptr; 
	mesh = par_shapes_create_cube();
	par_shapes_translate(mesh, position.x, position.y, position.z);
	par_shapes_scale(mesh, length, height, width);

	if (mesh != nullptr)
		LOG("Created a Cube in position: \nx = %.2f, y = %.2f, z = %.2f", position.x, position.y, position.y);

	uint id = 0; 
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->npoints * 3, mesh->points, GL_STATIC_DRAW);

	if (id != 0)
		LOG("Generated array buffer correctly. Buffer Id = %i", id);

	uint indexId = 0; 
	glGenBuffers(1, &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->ntriangles * 3, mesh->triangles, GL_STATIC_DRAW);

	if (indexId != 0)
		LOG("Generated element array buffer correctly. Buffer Id = %i",indexId);

	shapeInfo* shape = new shapeInfo(); 
	shape->myId = id; 
	shape->indexId = indexId; 
	shape->myMesh = mesh;
	
	shapes.push_back(shape); 
	LOG("Pushed shape to the list. Current shapes: %i", shapes.size());
}

void ModuleEditor::DrawShapes()
{
	if (shapes.empty())
		return; 

	std::list<shapeInfo*>::const_iterator item = shapes.begin();
	glEnableClientState(GL_VERTEX_ARRAY);

	for (item; item != shapes.end(); item++)
	{
		// Drawing Cube
		glBindBuffer(GL_ARRAY_BUFFER, (*item)->myId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->indexId);
		glDrawElements(GL_TRIANGLES, (*item)->myMesh->ntriangles * 3, GL_UNSIGNED_SHORT, (void*)0);
	}
	glDisableClientState(GL_VERTEX_ARRAY);

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


//void ModuleEditor::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

