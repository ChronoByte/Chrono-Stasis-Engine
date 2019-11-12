
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"
#include "csTextureLoader.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
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

	GameObject* go = App->fbx->LoadModel("Assets/BakerHouse/BakerHouse.FBX");

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
		App->fbx->LoadModel("Assets/BakerHouse/BakerHouse.FBX");
	

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
	BoundingBox boxParShape;

	switch (type)
	{
	case PrimitiveType::CUBE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cube");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cube(); 

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape); 
		go->AssignComponent(mesh); 
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::SPHERE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Sphere");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_parametric_sphere(20,20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::PLANE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Plane");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_plane(50, 50);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::CYLINDER:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cylinder");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cylinder(20, 20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::CONE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Cone");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_cone(20, 20);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::TORUS:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Torus");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_torus(10, 10, 1);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::KLEIN_BOTTLE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Klein Bottle");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_klein_bottle(30, 30);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::TREFOIL_KNOT:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Trefoil Knot");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_trefoil_knot(30, 30, 3);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::HEMISPHERE:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Hemisphere");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_hemisphere(10, 10);

		// Load into GameObject
		shape = par_shapes_create_hemisphere(10, 10);
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	case PrimitiveType::ROCK:
		// Create Object + Component + Shape
		go = CreateGameObject(parent, "Rock");
		mesh = new ComponentMesh(nullptr);
		shape = par_shapes_create_rock(8, 3);

		// Load into GameObject
		mesh->LoadMeshFromParShape(shape, &boxParShape);
		go->AssignComponent(mesh);
		boxParShape.box = AABB(boxParShape.min, boxParShape.max);
		go->GetTransform()->SetBoundingBox(boxParShape.box);
		break;

	default:
		break; 
	}

	par_shapes_free_mesh(shape);

	return go;
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