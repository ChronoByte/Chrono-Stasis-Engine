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

	//Adding flags using bit-wise OR

	CreateCubeParShapes();
	CreateSphereParShapes();

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

	par_shapes_free_mesh(cubeMesh); 
	par_shapes_free_mesh(sphereMesh); 

	
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
			if (ImGui::MenuItem("Test pcg", "", randomWin->GetBool())) {}
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


	//DrawCubeDirectMode();
	//DrawCubeVertexArray(); 
	//DrawCubeIndexArray(); 
	DrawCubeParShapes(); 
	DrawShapes();
	DrawSphereParShapes();

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


void ModuleEditor::CreateCubeParShapes()
{
	cubeMesh = par_shapes_create_cube();
	par_shapes_translate(cubeMesh, 1, 0, 0.5);
	par_shapes_scale(cubeMesh, 1.2, 1.2, 1.2);

	glGenBuffers(1, &cubeId);
	glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeMesh->npoints * 3, cubeMesh->points, GL_STATIC_DRAW);


	glGenBuffers(1, &indexCubeId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexCubeId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * cubeMesh->ntriangles * 3, cubeMesh->triangles, GL_STATIC_DRAW);
}

void ModuleEditor::CreateSphereParShapes()
{
	sphereMesh = par_shapes_create_subdivided_sphere(2); 
	par_shapes_translate(sphereMesh, 1, 1, -0.5);

	glGenBuffers(1, &sphereId); 
	glBindBuffer(GL_ARRAY_BUFFER, sphereId); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphereMesh->npoints * 3, sphereMesh->points, GL_STATIC_DRAW);

	glGenBuffers(1, &indexSphereId); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexSphereId); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * sphereMesh->ntriangles * 3, sphereMesh->triangles, GL_STATIC_DRAW); 
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
	glGenBuffers(1, &myId);
	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 9 * 2, vertices, GL_STATIC_DRAW);		// 6 faces * 9 vertex * 2 triangles 

	glColor3f(255, 0, 158);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, (6 * 9 * 2) / 3); // 6 faces * 9 vertex * 2 triangles / 3 vertex per trianble
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleEditor::DrawCubeIndexArray()
{
	float vertices[8 * 3] =
	{
		0.f, 0.f, 0.f,
		0.f, 0.f, 1.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 1.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f
	};

	uint index[6] =
	{
		0, 1, 2,
		1, 3, 2
	};

	GLuint myId = 0;
	glGenBuffers(1, &myId);
	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);

	uint indicesId = 0;
	glGenBuffers(1, &indicesId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, myId);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColor3f(255, 0, 158);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleEditor::DrawCubeParShapes()
{ 
	glEnableClientState(GL_VERTEX_ARRAY);


	// Drawing Cube
	glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexCubeId);
	glDrawElements(GL_TRIANGLES, cubeMesh->ntriangles * 3, GL_UNSIGNED_SHORT, (void*)0);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleEditor::DrawSphereParShapes()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, sphereId); 
	glVertexPointer(3, GL_FLOAT, 0, NULL); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexSphereId); 
	glDrawElements(GL_TRIANGLES, sphereMesh->ntriangles * 3, GL_UNSIGNED_SHORT, (void*)0);

	glDisableClientState(GL_VERTEX_ARRAY);
}



//void ModuleEditor::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

