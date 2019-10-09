#include "AboutWindow.h"
#include "csApp.h"
#include "csWindow.h"
#include "PhysFS/include/physfs.h"


AboutWindow::AboutWindow(bool startOpened) : Window(startOpened)
{	
}


AboutWindow::~AboutWindow() 
{
}


void AboutWindow::Draw()
{
	ImGui::Begin("About", &active);

	Introduction();
	MadeBy();
	SoftwareUsed();
	License();

	ImGui::End();
}

void AboutWindow::Introduction()
{
	ImGui::Text("Chrono Stasis Engine");
	ImGui::Text("  The ultimate 3D game engine.");
	ImGui::NewLine();
}

void AboutWindow::MadeBy()
{
	ImGui::Text("Made by:");
	ImGui::NewLine();
	ImGui::TextColored(ImVec4(255,0,100,255),"    Carlos Pena");
	if(ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::IsItemClicked())
		App->SendToLink("https://github.com/CarlosUPC");

	ImGui::NewLine();
	ImGui::TextColored(ImVec4(255, 0, 100, 255), "    Sebi Lopez");
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://github.com/Sebi-Lopez");

	ImGui::NewLine(); 
	ImGui::NewLine(); 
}

void AboutWindow::SoftwareUsed()
{
	ImGui::Text("3rd Parties Libraries Used: ");

	SDL_version compiled;
	SDL_VERSION(&compiled);
	ImGui::Text("  SDL - Simple Direct Media %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://www.libsdl.org");

	ImGui::Text("  ImGui v%s", ImGui::GetVersion());	
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://github.com/ocornut/imgui");

	ImGui::Text("  MathGeoLib v1.5");
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://github.com/juj/MathGeoLib");

	ImGui::Text("  Glew v%s", glewGetString(GLEW_VERSION));
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("http://glew.sourceforge.net/");

	ImGui::Text("  OpenGL v%s", glGetString(GL_VERSION));
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://www.opengl.org/");

	ImGui::Text("  PhysFS v%i.%i.%i", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://icculus.org/physfs/");

	ImGui::Text("  DevIL (Developer's Image Library) v%i.%i.%i", 1, 8, 0);
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("http://openil.sourceforge.net/");

	ImGui::NewLine();
}

void AboutWindow::License()
{
	ImGui::NewLine();
	ImGui::Text("License:");
	ImGui::NewLine();
	ImGui::TextWrapped("MIT License \n\n\nCopyright(c) 2019 ChronoByte \n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
}
