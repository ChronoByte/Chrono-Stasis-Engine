
#include "csApp.h"
#include "csScene.h"
#include "csFBXLoader.h"
#include "csTextureLoader.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "csViewport.h"

#include "csGameObject.h"

#include "src/Structure/GameViewWindow.h"

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
	CreateRoot(); 
	
	App->fbx->LoadModel("Assets/Models/BakerHouse.FBX");

	CleanSelected(); 

	octree = new Octree(AABB(float3(-50.f, -50.f, -50.f), float3(50.f,50.f, 50.f)));

	return true;
}

bool ModuleScene::CleanUp()
{
	delete root;
	root = nullptr; 

	delete octree; 
	octree = nullptr; 

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->fbx->LoadModel("Assets/Models/BakerHouse/BakerHouse.FBX");

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		activeOctree = !activeOctree;

	return UPDATE_CONTINUE;
}


update_status ModuleScene::Update(float dt)
{

	UpdateAllGameObjects(root, dt);

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::DrawScene()
{

	DrawAllGameObjects(root);

}

void ModuleScene::DebugDrawScene()
{

	if (App->renderer3D->GetDebugMode())
	{
		DrawGrid();
		DrawOriginAxis();
	}
	App->camera->DrawMouseRay();
	DebugDrawAllGameObjects(root);

	if(isOctreeActive() && drawOctree)
		DrawOctree();

}

void ModuleScene::DrawGrid()
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

void ModuleScene::DrawOriginAxis()
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

void ModuleScene::DrawOctree()
{
	std::vector<OctreeNode*> nodes;
	octree->CollectZones(nodes);

	glBegin(GL_LINES);
	glColor3f(0.f, 1.f, 0.f);

	for (uint j = 0; j < nodes.size(); ++j)
	{
		for (uint i = 0; i < nodes[j]->zone.NumEdges(); ++i)
		{
			glVertex3f(nodes[j]->zone.Edge(i).a.x, nodes[j]->zone.Edge(i).a.y, nodes[j]->zone.Edge(i).a.z);
			glVertex3f(nodes[j]->zone.Edge(i).b.x, nodes[j]->zone.Edge(i).b.y, nodes[j]->zone.Edge(i).b.z);
		}
	}

	glEnd();
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

	if (go != nullptr)
		SetSelected(go);
	return go;
}

GameObject * ModuleScene::CreateCamera(GameObject * parent, const char * name)
{
	GameObject* go = CreateGameObject(parent, name); 
	go->CreateComponent(ComponentType::C_CAMERA); 
	SetSelected(go);

	// Set a position to the camera so it doesn't spawn with akward view
	go->GetTransform()->SetPosition(float3(0.f, 10.f, -20.f));
	go->GetTransform()->SetRotationEuler(float3(20.f, 0.f, 0.f));

	return go;
}

GameObject * ModuleScene::GetSelected() const
{
	return selected;
}

void ModuleScene::SetSelected(GameObject* go)
{
	selected = go; 
}

void ModuleScene::SetRoot(GameObject* go)
{
	root = go;
}

void ModuleScene::CleanSelected()
{
	selected = nullptr; 
}

void ModuleScene::DeleteRoot()
{
	delete root;
	root = nullptr;
}

void ModuleScene::SetMainCamera(ComponentCamera * camera)
{
	if (camera == nullptr)
		return; 

	if (mainCamera != nullptr)
		mainCamera->isMainCamera = false;

	mainCamera = camera; 
	mainCamera->isMainCamera = true; 

	// Set game viewport camera
	if (App->renderer3D->gameViewport != nullptr)
		App->renderer3D->gameViewport->SetCamera(mainCamera); 

	// Open window
	App->editor->gameView->SetActive(true); 
}

void ModuleScene::ClearCamera()
{
	if (mainCamera != nullptr)
		mainCamera->isMainCamera = false; 

	mainCamera = nullptr;

	// Set game viewport camera
	if(App->renderer3D->gameViewport != nullptr)
		App->renderer3D->gameViewport->SetCamera(nullptr);

	// Close window
	App->editor->gameView->SetActive(false);
}

ComponentCamera * ModuleScene::GetMainCamera() const
{
	return mainCamera;
}

bool ModuleScene::isOctreeActive() const
{
	return activeOctree;
}

GameObject * ModuleScene::CreateGameObject(GameObject* parent, const char* name)
{
	if (parent == nullptr)
		parent = root; 

	GameObject* go = new GameObject(parent); 
	go->SetName(name); 
	SetSelected(go);
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

void ModuleScene::UpdateAllGameObjects(GameObject * parent, float dt)
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
			UpdateAllGameObjects((*it), dt);
			it++;
		}
	}
}

void ModuleScene::DrawAllGameObjects(GameObject * parent)
{
	if (!parent->isActive())
		return;

	parent->OnDraw();

	std::list<GameObject*>::iterator it = parent->childs.begin();
	for (it; it != parent->childs.end(); ++it)
	{
		DrawAllGameObjects((*it)); 
	}
}

void ModuleScene::DebugDrawAllGameObjects(GameObject * parent)
{
	if (!parent->isActive())
		return;

	parent->OnDebugDraw();

	std::list<GameObject*>::iterator it = parent->childs.begin();
	for (it; it != parent->childs.end(); ++it)
	{
		DebugDrawAllGameObjects((*it));
	}

}

void ModuleScene::CheckRayAgainstAABBS(GameObject * parent, const LineSegment& ray, std::multimap<float, GameObject*>& objectsIntersected, int& tests)
{
	float nearDist = 0;
	float farDist = 0;

	bool intersects = ray.Intersects(parent->GetTransform()->GetBoundingBox(), nearDist, farDist); 
	tests++; 

	if (intersects)
		objectsIntersected.insert(std::pair<float, GameObject*>(nearDist, parent));
	
	for (std::list<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
	{
		CheckRayAgainstAABBS((*it), ray, objectsIntersected, tests);
	}
}

GameObject* ModuleScene::CheckRayAgainstTris(const LineSegment& ray, const std::multimap<float, GameObject*>& intersected)
{
	GameObject* firstObjectHit = nullptr; 
	float minDist = FLOAT_INF;

	std::multimap<float, GameObject*>::const_iterator iter = intersected.begin(); 

	for (iter; iter != intersected.end(); ++iter)
	{
		GameObject* gameObject = (*iter).second; 
		ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(gameObject->FindComponent(ComponentType::C_MESH));
		bool hit = false; 

		if (mesh == nullptr)
			continue; 

		// Transform ray into local space
		float4x4 inverted_m = gameObject->GetTransform()->GetGlobalTransform().Inverted();
		LineSegment rayLocal = inverted_m * ray;

		for (uint i = 0; i < mesh->index.capacity / 3;)
		{
			float3 a, b, c;

			a = float3(&mesh->vertex.buffer[mesh->index.buffer[i++] * 3]);
			b = float3(&mesh->vertex.buffer[mesh->index.buffer[i++] * 3]);
			c = float3(&mesh->vertex.buffer[mesh->index.buffer[i++] * 3]);

			Triangle tri = Triangle(a, b, c); 
			//tri.Transform(gameObject->GetTransform()->GetGlobalTransform());
			float distanceHit = 0.f; 

			hit = rayLocal.Intersects(tri, &distanceHit, nullptr);

			if (hit)
			{
				LOG("Hit with a triangle in the mesh of %s and at distance: %f and hitpoint %f", gameObject->GetName(), distanceHit);
				if (distanceHit < minDist)
				{
					LOG("New lowest distance ^");
					minDist = distanceHit;
					firstObjectHit = gameObject; 
				}
			
			}
		}

		// If a triangle is hit, there's no need to look further
		if (hit)
			break; 
	}

	if (firstObjectHit != nullptr)
		LOG("Success mouse picking: Object Selected = %s", firstObjectHit->GetName());

	return firstObjectHit; 
}