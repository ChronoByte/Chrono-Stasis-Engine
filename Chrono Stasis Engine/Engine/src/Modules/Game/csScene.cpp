
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"
#include "csTextureLoader.h"
#include "ComponentMesh.h"
#include "csGameObject.h"

#include "GLEW/include/GL/glew.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "Scene"; 
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
	ComponentMesh* mesh = nullptr;
	TextureInfo* tex = nullptr; 
	uint texture = 0; 

	CreateRoot(); 

	App->fbx->LoadFBXData("Assets/BakerHouse.FBX");
	tex = App->texture->LoadTexture("Assets/Baker_house.tga");

	if (tex != nullptr && mesh != nullptr)
		mesh->AssignTexture(tex); 


	return true;
}

bool ModuleScene::CleanUp()
{
	delete root; 

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

	//root->Update(dt); 
	RecursiveUpdate(root, dt);
	//DirectDrawing(imageId);

	return UPDATE_CONTINUE;
}

void ModuleScene::DirectDrawing(const uint &imageId)
{
	glBindTexture(GL_TEXTURE_2D, imageId);
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

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnd();
}



GameObject * ModuleScene::CreateGameObject(GameObject* parent, const char* name)
{
	if (parent == nullptr)
		parent = root; 

	GameObject* go = new GameObject(parent); 
	go->SetName(name); 
	return go;
}

GameObject * ModuleScene::CreateRoot()
{
	root = new GameObject(nullptr);
	root->SetName("Root");
	return root; 
}

void ModuleScene::RecursiveUpdate(GameObject * parent, float dt)
{
	parent->Update(dt);

	for (std::list<GameObject*>::const_iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
	{
		RecursiveUpdate((*it), dt); 
	}
}
