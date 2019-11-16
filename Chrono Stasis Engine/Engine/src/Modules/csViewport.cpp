#include "csApp.h"
#include "csViewport.h"
#include "csRenderer3D.h"

Viewport::Viewport()
{
}

Viewport::~Viewport()
{
}


void Viewport::PreUpdate()
{
	App->renderer3D->PrepareDraw(this);
}

void Viewport::Update()
{
	App->renderer3D->Draw(this);
}

bool Viewport::CleanUp()
{
	return true;
}
