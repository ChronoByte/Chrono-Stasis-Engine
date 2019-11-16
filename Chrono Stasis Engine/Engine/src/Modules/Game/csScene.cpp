
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"
#include "csTextureLoader.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "csGameObject.h"
#include "csViewport.h"

#include "src/Structure/GameViewWindow.h"

#include "GLEW/include/GL/glew.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "Scene"; 
	sceneViewport = new Viewport(); 
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

	GameObject* go = App->fbx->LoadModel("Assets/Models/BakerHouse/BakerHouse.FBX");

	return true;
}

update_status ModuleScene::PreUpdate()
{
	//if(sceneViewport->camera != nullptr)
		//sceneViewport->PreUpdate(); 
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	delete root; 

	return true;
}

update_status ModuleScene::Update(float dt)
{

	/*if (sceneViewport->camera != nullptr)
		sceneViewport->Update();*/

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->fbx->LoadModel("Assets/Models/BakerHouse/BakerHouse.FBX");
	

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

	RecursiveUpdate(root, dt);

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

	GameObject* go = nullptr;
	ComponentMesh* mesh = nullptr;
	par_shapes_mesh* shape = nullptr;

	switch (type)
	{
	case PrimitiveType::CUBE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cube");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cube(); 

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape); 
		go->AssignComponent(mesh); 
		break;

	case PrimitiveType::SPHERE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Sphere");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_parametric_sphere(20,20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::PLANE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Plane");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_plane(50, 50);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::CYLINDER:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cylinder");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cylinder(20, 20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::CONE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cone");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cone(20, 20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::TORUS:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Torus");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_torus(10, 10, 1);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::KLEIN_BOTTLE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Klein Bottle");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_klein_bottle(30, 30);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::TREFOIL_KNOT:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Trefoil Knot");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_trefoil_knot(30, 30, 3);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::HEMISPHERE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Hemisphere");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_hemisphere(10, 10);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	case PrimitiveType::ROCK:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Rock");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_rock(8, 3);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape);
		go->AssignComponent(mesh);
		break;

	default:
		break; 
	}

	par_shapes_free_mesh(shape);

	return go;
}

GameObject * ModuleScene::CreateCamera(GameObject * parent, const char * name)
{
	GameObject* go = CreateGameObject(parent, name); 
	go->CreateComponent(ComponentType::C_CAMERA); 
	return go;
}

void ModuleScene::SetMainCamera(ComponentCamera * camera)
{
	if (camera == nullptr)
		return; 

	if (mainCamera != nullptr)
		mainCamera->isMainCamera = false;

	mainCamera = camera; 
	sceneViewport->camera = mainCamera; 
	App->renderer3D->OnResize(App->window->width, App->window->height, sceneViewport); 
	mainCamera->isMainCamera = true; 

	App->editor->gameView->SetActive(true); 
}

void ModuleScene::ClearCamera()
{
	if (mainCamera != nullptr)
		mainCamera->isMainCamera = false; 

	mainCamera = nullptr; 
	sceneViewport->camera = nullptr;
	App->editor->gameView->SetActive(false);
}

ComponentCamera * ModuleScene::GetMainCamera() const
{
	return mainCamera;
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

	std::list<GameObject*>::iterator it = parent->childs.begin();
	while (it != parent->childs.end())
	{
		if ((*it)->to_delete)
		{
			delete (*it);
			it = parent->childs.erase(it);
		}
		else
		{
			RecursiveUpdate((*it), dt);
			it++;
		}
	}
}