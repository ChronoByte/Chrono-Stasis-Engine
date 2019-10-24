
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"
#include "csTextureLoader.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
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
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateRoot(); 

	GameObject* go = App->fbx->LoadModel("Assets/BakerHouse.FBX");

	return true;
}

update_status ModuleScene::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	delete root; 

	return true;
}

update_status ModuleScene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->fbx->LoadModel("Assets/BakerHouse.FBX");
	

	if (App->renderer3D->GetDebugMode())
	{
		glLineWidth(1.0f);

		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

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

update_status ModuleScene::PostUpdate()
{
	return UPDATE_CONTINUE;
}

GameObject * ModuleScene::CreateObject3D(PrimitiveType type, GameObject * parent)
{
	if (type == PrimitiveType::MAX)
		return nullptr;

	GameObject* go = CreateGameObject(parent, "Primitive");
	ComponentMesh* mesh = new ComponentMesh(nullptr);

	par_shapes_mesh* shape = nullptr;

	switch (type)
	{
	case PrimitiveType::CUBE:

		shape = par_shapes_create_cube(); 
		mesh->LoadMeshFromParShape(shape); 
		go->AssignComponent(mesh); 
		break;

	case PrimitiveType::SPHERE:
		shape = par_shapes_create_parametric_sphere(20,20);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::PLANE:
		shape = par_shapes_create_plane(50, 50);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::CYLINDER:
		shape = par_shapes_create_cylinder(20,20);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::CONE:
		shape = par_shapes_create_cone(20, 20);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::TORUS:
		shape = par_shapes_create_torus(10, 10, 1);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::KLEIN_BOTTLE:
		shape = par_shapes_create_klein_bottle(30, 30);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::TREFOIL_KNOT:
		shape = par_shapes_create_trefoil_knot(30, 30, 3);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::HEMISPHERE:
		shape = par_shapes_create_hemisphere(10, 10);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::ROCK:
		shape = par_shapes_create_rock(8, 3);
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	default:
		break; 
	}

	par_shapes_free_mesh(shape);

	return go;
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

GameObject * ModuleScene::GetRoot() const
{
	return root;
}

void ModuleScene::RecursiveUpdate(GameObject * parent, float dt)
{
	if (!parent->isActive())
		return;

	parent->Update(dt);

	for (std::list<GameObject*>::const_iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
	{
		RecursiveUpdate((*it), dt);
	}
	
}