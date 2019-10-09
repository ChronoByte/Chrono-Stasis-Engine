
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
	

	if (App->renderer3D->GetDebugMode())
	{
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
	}



	glBegin(GL_TRIANGLES);
	//glColor4f(255, 0, 128, 255);

	// Face 1
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);

	// Face 2
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(1.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(0.0f, 1.0f);


	// Face 3
	glVertex3f(1.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);

	glVertex3f(1.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 1.f, 1.f);
	glTexCoord2f(0.0f, 1.0f);

	// Face 4
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 0.0f);

	// Face 5
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.f, 1.f, 0.f); 
	glTexCoord2f(0.0f, 1.0f);

	glVertex3f(1.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);


	// Face 6
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);

	glVertex3f(1.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 1.0f);

	glEnd();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}