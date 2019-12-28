#include "csApp.h"
#include "csComponent.h"


void Component::AssignResource(UID uid)
{
	if (currentResource != nullptr)
		currentResource->UnloadFromMemory();

	currentResource = App->resources->GetResource(uid);

	/*if (currentResource)
		currentResource->LoadToMemory();*/
	
}

void Component::AssignParticleResource(UID uid)
{
	currentResource = App->resources->GetResource(uid);
}
