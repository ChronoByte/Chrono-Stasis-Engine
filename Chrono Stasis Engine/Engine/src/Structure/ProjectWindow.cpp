#include "ProjectWindow.h"

ProjectWindow::ProjectWindow(bool startOpened) : Window(startOpened)
{
}

ProjectWindow::~ProjectWindow()
{
}

void ProjectWindow::Draw()
{
	ImGui::Begin("Project", &active);
	
	ImGui::Text("LOLAS QUE HACES MIRANDO NUESTRO CODIGO DEL ENGINE?");
	ImGui::Text("PILLLIIIIIIIIIIIIIIIIIIIIIIN");
	ImGui::End();
}
