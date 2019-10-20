#include "GeometryWindow.h"
#include "csApp.h"
#include "csWindow.h"

GeometryWindow::GeometryWindow(bool startOpened) : Window(startOpened)
{
}

GeometryWindow::~GeometryWindow() 
{
}


void GeometryWindow::Draw()
{
	ImGui::Begin("Geometry Creation");
	ImGui::End();
}
