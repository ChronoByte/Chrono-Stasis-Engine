
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

	root = CreateGameObject("Root"); 
	GameObject* child1 = CreateGameObject("Child 1"); 
	child1->SetParent(root); 

	GameObject* child2 = CreateGameObject("Child 2");
	child2->SetParent(child1); 

	child2->SetParent(child1); 
	child1->SetParent(child2); 







	mesh = App->fbx->LoadFBXData("Assets/BakerHouse.FBX");
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

	
	/*GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}*/

	/*uint imageId = 0; 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &imageId);
	glBindTexture(GL_TEXTURE_2D, imageId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/



	//Mesh* mesh = new Mesh();


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



GameObject * ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go = nullptr; 
	go = new GameObject(); 
	go->SetName(name); 
	return go;
}
