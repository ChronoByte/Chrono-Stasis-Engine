#include "csGlobals.h"
#include "csApp.h"
#include "csEditor.h"
#include <string>
#include "time.h"
#include "csInput.h"
#include "imgui/imgui.h"


#include "ComponentMesh.h"

// Including windows
#include "src/Structure/ConfigWindow.h"
#include "src/Structure/CollisionWindow.h"
#include "src/Structure/AboutWindow.h"
#include "src/Structure/ConsoleWindow.h"
#include "src/Structure/RandomWindow.h"
#include "src/Structure/InspectorWindow.h"
#include "src/Structure/HierarchyWindow.h"
#include "src/Structure/GeometryWindow.h"
#include "src/Structure/SceneViewWindow.h"

ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	name = "Editor";
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init(JSON_Object* node)
{

	windows.push_back(inspector = new InspectorWindow(json_object_get_boolean(node, "inspector")));
	windows.push_back(hierarchy = new HierarchyWindow(json_object_get_boolean(node, "hierarchy")));
	windows.push_back(console = new ConsoleWindow(json_object_get_boolean(node, "console")));
	windows.push_back(config = new ConfigWindow(json_object_get_boolean(node, "configuration")));
	windows.push_back(about = new AboutWindow(json_object_get_boolean(node, "about")));
	windows.push_back(geometryWin = new GeometryWindow(json_object_get_boolean(node, "geometry")));
	windows.push_back(randomWin = new RandomWindow(json_object_get_boolean(node, "random")));
	windows.push_back(collisionWin = new CollisionWindow(json_object_get_boolean(node, "collision")));
	windows.push_back(sceneView = new SceneViewWindow(json_object_get_boolean(node, "scene view")));

	return true;
}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//START ALL WINDOWS
	std::vector<Window*>::iterator item = windows.begin();
	for (int i = 0; i < windows.size(); i++)
		item[i]->Start();
	
	
	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	//CLEANUP ALL WINDOWS
	std::vector<Window*>::iterator item = windows.begin();
	
	for (int i = 0; i < windows.size(); i++) {
		delete item[i];
	}

	windows.clear(); 

	config = nullptr; 
	collisionWin = nullptr; 
	about = nullptr; 
	console = nullptr; 
	randomWin = nullptr; 
	hierarchy = nullptr; 
	geometryWin = nullptr; 

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

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close", "Ctrl+W")) { App->to_close_app = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo Window", "", &show_demo_window)) {}
		    if (ImGui::MenuItem("Style Window", "", &show_style_window)) {}
			if (ImGui::MenuItem("Console", "", console->GetBool()))	{}
			if (ImGui::MenuItem("Inspector", "", inspector->GetBool())) {}
			if (ImGui::MenuItem("Hierarchy", "", hierarchy->GetBool())) {}
			if (ImGui::MenuItem("Configuration", "", config->GetBool())) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem("Create Empty", "")) { App->scene->CreateGameObject(nullptr, "GameObject");  }
			if (ImGui::BeginMenu("3D Object")) 
			{
				if (ImGui::MenuItem("Cube")) { App->scene->CreateObject3D(PrimitiveType::CUBE, nullptr); }
				if (ImGui::MenuItem("Sphere")) { App->scene->CreateObject3D(PrimitiveType::SPHERE, nullptr); }
				if (ImGui::MenuItem("Plane")) { App->scene->CreateObject3D(PrimitiveType::PLANE, nullptr); }
				if (ImGui::MenuItem("Cylinder")) { App->scene->CreateObject3D(PrimitiveType::CYLINDER, nullptr); }
				if (ImGui::MenuItem("Cone")) { App->scene->CreateObject3D(PrimitiveType::CONE, nullptr); }
				if (ImGui::MenuItem("Torus")) { App->scene->CreateObject3D(PrimitiveType::TORUS, nullptr); }
				if (ImGui::MenuItem("Klein Bottle")) { App->scene->CreateObject3D(PrimitiveType::KLEIN_BOTTLE, nullptr); }
				if (ImGui::MenuItem("Trefoil Knot")) { App->scene->CreateObject3D(PrimitiveType::TREFOIL_KNOT, nullptr); }
				if (ImGui::MenuItem("Hemisphere")) { App->scene->CreateObject3D(PrimitiveType::HEMISPHERE, nullptr); }
				if (ImGui::MenuItem("Rock")) { App->scene->CreateObject3D(PrimitiveType::ROCK, nullptr); }
				ImGui::Separator();
				if (ImGui::MenuItem("Creation Options", "", geometryWin->GetBool())) {}

				ImGui::EndMenu(); 
			}
			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Collision Tester", "", collisionWin->GetBool())) {}
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


	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (show_style_window) {
		ImGui::Begin("Style Editor", &show_style_window);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
		
	App->gui->DockLoader();
	//DRAW ALL WINDOWS
	std::vector<Window*>::iterator item = windows.begin();
	for (int i = 0; i < windows.size(); i++)
	{
			if (item[i]->GetActive())
				item[i]->Draw();
	}
	
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

	LOG("Created a Cube in position: \nx = %.2f, y = %.2f, z = %.2f", position.x, position.y, position.y);

	ComponentMesh* shape = new ComponentMesh(nullptr);

	//App->renderer3D->PushMeshToRender(shape);
}

