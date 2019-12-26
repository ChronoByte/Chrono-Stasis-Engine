
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

#include "csResources.h"

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
	CreateOctree(OCTREE_SIZE);

	//CreateCamera(nullptr, "Main Camera");

	ClearScene();
	App->serialization->LoadScene("Assets/Scenes/StreetScene.scene.json");
	

	CleanSelected(); 

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
	if (GetSelected() != nullptr && App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		GetSelected()->to_delete = true;


	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		activeOctree = !activeOctree;
	

	if (toRecreateOctree)
	{
		toRecreateOctree = false;
		RecreateOctree(); 
	}

	return UPDATE_CONTINUE;
}


update_status ModuleScene::Update(float dt)
{

	UpdateAllGameObjects(root, dt);

	if (App->gameState == GameState::ONPLAY)
		OnGameUpdate(App->gameDt);

	/*LOG("---");
	for (std::list<GameObject*>::iterator iter = dynamicGameObjects.begin(); iter != dynamicGameObjects.end(); ++iter)
	{
		LOG("Dynamic Object: %s", (*iter)->GetName());
	}
	for (std::list<GameObject*>::iterator iter = staticGameObjects.begin(); iter != staticGameObjects.end(); ++iter)
	{
		LOG("Static Object: %s", (*iter)->GetName());
	}*/
	return UPDATE_CONTINUE;
}

void ModuleScene::OnGameUpdate(float dt)
{
	std::vector<GameObject*> objects; 
	objects.push_back(root); 

	while (!objects.empty())
	{
		GameObject* current = objects.back();
		objects.pop_back(); 
		objects.insert(objects.end(), current->childs.begin(), current->childs.end()); 

		current->OnGameUpdate(dt); 
	}

	objects.clear(); 
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::DrawScene()
{
	if (App->scene->mainCamera != nullptr && App->scene->mainCamera->isCulling())
	{
		if(isOctreeActive())
			DrawCullingObjectsWithOctree();
		else
			DrawCullingAllObjects();

	}
	else DrawAllGameObjects(root);

}

void ModuleScene::DrawCullingAllObjects()
{
	int intersections = 0;
	std::vector<GameObject*> objects;
	objects.push_back(root);

	while (!objects.empty())
	{
		GameObject* current = objects.back();
		objects.pop_back();
		objects.insert(objects.end(), current->childs.begin(), current->childs.end());

		if (App->scene->mainCamera->CheckAABBInsideFrustum(current->GetTransform()->GetBoundingBox()))
			current->OnDraw();

		intersections++;
	}
	//LOG("Checking %i intersections without octree", intersections);
}

void ModuleScene::DrawCullingObjectsWithOctree()
{
	std::vector<GameObject*> candidates;
	octree->CollectCandidates(candidates, App->scene->mainCamera->frustum);
	int intersections = 0; 

	for (uint i = 0; i < candidates.size(); ++i)
	{
		if (App->scene->mainCamera->CheckAABBInsideFrustum(candidates[i]->GetTransform()->GetBoundingBox()))
			candidates[i]->OnDraw();

		intersections++;
	}

	for (std::list<GameObject*>::const_iterator iter = dynamicGameObjects.begin(); iter != dynamicGameObjects.end(); ++iter)
	{
		if (App->scene->mainCamera->CheckAABBInsideFrustum((*iter)->GetTransform()->GetBoundingBox()))
			(*iter)->OnDraw();

		intersections++;

	}
	//LOG("Checking %i intersections with octree", intersections);
	candidates.clear();
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
	if (octree->GetRoot() == nullptr)
		return; 

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

	nodes.clear(); 

	glColor3f(1.f, 1.f, 1.f);

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
		go = CreateGameObject(parent, "Cube");
		shape = par_shapes_create_cube(); 
		break;

	case PrimitiveType::SPHERE:
		go = CreateGameObject(parent, "Sphere");
		shape = par_shapes_create_parametric_sphere(20,20);
		break;

	case PrimitiveType::PLANE:
		go = CreateGameObject(parent, "Plane");
		shape = par_shapes_create_plane(50, 50);
		break;

	case PrimitiveType::CYLINDER:
		go = CreateGameObject(parent, "Cylinder");
		shape = par_shapes_create_cylinder(20, 20);
		break;

	case PrimitiveType::CONE:
		go = CreateGameObject(parent, "Cone");
		shape = par_shapes_create_cone(20, 20);
		break;

	case PrimitiveType::TORUS:
		go = CreateGameObject(parent, "Torus");
		shape = par_shapes_create_torus(10, 10, 1);
		break;

	case PrimitiveType::KLEIN_BOTTLE:
		go = CreateGameObject(parent, "Klein Bottle");
		shape = par_shapes_create_klein_bottle(30, 30);
		break;

	case PrimitiveType::TREFOIL_KNOT:
		go = CreateGameObject(parent, "Trefoil Knot");
		shape = par_shapes_create_trefoil_knot(30, 30, 3);
		break;

	case PrimitiveType::HEMISPHERE:
		go = CreateGameObject(parent, "Hemisphere");
		shape = par_shapes_create_hemisphere(10, 10);
		break;

	case PrimitiveType::ROCK:
		go = CreateGameObject(parent, "Rock");
		shape = par_shapes_create_rock(8, 3);
		break;

	default:
		break; 
	}

	if (go == nullptr || shape == nullptr)
		return nullptr; 

	// Create component mesh and assign
	mesh = new ComponentMesh(nullptr); 
	go->AssignComponent(mesh); 

	// Create resource and load info
	ResourceMesh* res = (ResourceMesh*)App->resources->CreateNewResource(Resource::Type::R_MESH, 0);
	res->LoadMeshFromParShape(shape);
	
	// Save Resource info
	/*std::string outputfile;
	App->fbx->SaveMesh(res, res->GetUID(), outputfile);
	res->SetExportedFile(outputfile.c_str());*/

	// Finally assign resource to the mesh
	mesh->AssignResource(res->GetUID());


	// ---------------------------------
	par_shapes_free_mesh(shape);

	if (go != nullptr)
	{
		SetSelected(go);
		PushToDynamic(go);
	}
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

GameObject * ModuleScene::CreateParticleSystem(GameObject * parent, const char * name)
{
	GameObject* go = CreateGameObject(parent, name);
	go->CreateComponent(ComponentType::C_PARTICLE_SYSTEM);
	SetSelected(go);

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

void ModuleScene::CleanSelected()
{
	selected = nullptr;
}

void ModuleScene::DeleteRoot()
{
	delete root;
	root = nullptr;
}

void ModuleScene::ClearScene()
{
	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			delete (*it); 
			(*it) = nullptr; 
		}
	}
	root->childs.clear();

}

void ModuleScene::SetRoot(GameObject* go)
{
	root = go;
}

// ------------------ Camera ------------------------

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
	if (App->renderer3D->gameViewport != nullptr)
		App->renderer3D->gameViewport->SetCamera(nullptr);

	// Close window
	App->editor->gameView->SetActive(false);
}

ComponentCamera * ModuleScene::GetMainCamera() const
{
	return mainCamera;
}

// ------------------ Octree ------------------------

void ModuleScene::CreateOctree(const float & size)
{
	octree = new Octree(AABB(float3(-size, -size, -size), float3(size, size, size)));
}

void ModuleScene::InsertInOctree(GameObject * go)
{
	if (octree->GetRoot() == nullptr)		
		CreateOctree(OCTREE_SIZE); // Recreate Octree

	octree->Insert(go);

}

void ModuleScene::RemoveFromOctree(GameObject * go)
{
	octree->Remove(go);
}

void ModuleScene::ResetOctree()
{
	octree->ClearOctree();
}

void ModuleScene::RecreateOctree()
{
	octree->ClearOctree(); 
	std::list<GameObject*>::const_iterator iter = staticGameObjects.begin();
	for (iter; iter != staticGameObjects.end(); ++iter)
	{
		InsertInOctree((*iter)); 
	}
}

bool ModuleScene::isOctreeActive() const
{
	return activeOctree;
}

void ModuleScene::PushToDynamic(GameObject * go)
{
	dynamicGameObjects.push_back(go);
}

void ModuleScene::RemoveFromDynamic(GameObject * go)
{
	dynamicGameObjects.remove(go);
}

std::list<GameObject*> ModuleScene::GetDynamicObjects() const
{
	return dynamicGameObjects;
}

void ModuleScene::PushToStatic(GameObject * go)
{
	staticGameObjects.push_back(go); 
	toRecreateOctree = true; 
}

void ModuleScene::RemoveFromStatic(GameObject * go)
{
	staticGameObjects.remove(go); 
	toRecreateOctree = true;
}

std::list<GameObject*> ModuleScene::GetStaticObjects() const
{
	return staticGameObjects;
}

GameObject * ModuleScene::CreateGameObject(GameObject* parent, const char* name, bool import)
{
	if (parent == nullptr && import == false)
		parent = root; 

	GameObject* go = new GameObject(parent); 
	go->SetName(name); 
	SetSelected(go);
	PushToDynamic(go);
	return go;
}

GameObject* ModuleScene::LoadGameObject(GameObject* parent, const char* name, UID uid)
{
	if (parent == nullptr)
		parent = root;

	GameObject* go = new GameObject(parent, uid);
	go->SetName(name);
	SetSelected(go);
	PushToDynamic(go);
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

void ModuleScene::CheckRayAgainstAABBSRecursive(GameObject * parent, const LineSegment& ray, std::multimap<float, GameObject*>& objectsIntersected, int& tests)
{
	float nearDist = 0;
	float farDist = 0;

	bool intersects = ray.Intersects(parent->GetTransform()->GetBoundingBox(), nearDist, farDist); 
	tests++; 

	if (intersects)
		objectsIntersected.insert(std::pair<float, GameObject*>(nearDist, parent));
	
	for (std::list<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
	{
		CheckRayAgainstAABBSRecursive((*it), ray, objectsIntersected, tests);
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

		ComponentMesh* cmesh = dynamic_cast<ComponentMesh*>(gameObject->FindComponent(ComponentType::C_MESH));
		if (cmesh == nullptr)
			continue; 

		ResourceMesh* mesh = (ResourceMesh*)cmesh->GetCurrentResource();
		if (mesh == nullptr)
			continue; 

		bool hit = false; 

		// Transform ray into local space
		float4x4 inverted_m = gameObject->GetTransform()->GetGlobalTransform().Inverted();
		LineSegment rayLocal = inverted_m * ray;

		for (uint i = 0; i < mesh->GetIndicesSize();)
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
				//LOG("Hit with a triangle in the mesh of %s and at distance: %f and hitpoint %f", gameObject->GetName(), distanceHit);
				if (distanceHit < minDist)
				{
					//LOG("New lowest distance ^");
					minDist = distanceHit;
					firstObjectHit = gameObject; 
				}
			
			}
		}

		// If a triangle is hit in this game object, there's no need to look further
		if (hit)
			break; 
	}

	if (firstObjectHit != nullptr)
		LOG("Success mouse picking: Object Selected = %s", firstObjectHit->GetName());

	return firstObjectHit; 
}