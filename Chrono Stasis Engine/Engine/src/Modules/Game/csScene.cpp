
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}


bool ModuleScene::Init(JSON_Object* node)
{
	return true;
}
bool ModuleScene::Start()
{
	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->fbx->LoadFBXData("Assets/warrior.FBX");
	

	glLineWidth(1.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

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


	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}