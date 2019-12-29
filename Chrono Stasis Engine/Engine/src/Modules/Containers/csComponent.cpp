#include "csApp.h"
#include "csComponent.h"


void Component::AssignResource(UID uid)
{
	if (currentResource != nullptr)
		currentResource->UnloadFromMemory();

	currentResource = App->resources->GetResource(uid);
	
	if (currentResource)
		currentResource->LoadToMemory();
	
}

void Component::LoadResource(Resource* res)
{
	if (currentResource == nullptr)
		currentResource = res;
}

void Component::AssignParticleResource(UID uid)
{
	if (currentResource != nullptr)
		currentResource->UnloadFromMemory();

	currentResource = App->resources->GetResource(uid);
	


	/*if (currentResource)
		currentResource->LoadToMemory();*/

	/*if(currentResource->CountReferences < 2)
		currentResource->LoadToMemory();*/
}
