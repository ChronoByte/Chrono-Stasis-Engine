#include "AboutWindow.h"
#include "csApp.h"
#include "csWindow.h"

AboutWindow::AboutWindow(Application* app) : Window(app)
{	
}


AboutWindow::~AboutWindow() 
{
}


void AboutWindow::Draw()
{
	About();
}

void AboutWindow::About()
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
	ImGui::NewLine();
	ImGui::Text("The ultimate 3D game engine.");
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
}

void AboutWindow::SoftwareUsed()
{
	ImGui::Text("3rd Parties Libraries Used: ");
	ImGui::Text("  SDL - Simple Direct Media v2.0"); 
	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);
	if (ImGui::IsItemClicked())
		App->SendToLink("https://www.libsdl.org");
	ImGui::Text("  ETC...");
	ImGui::NewLine();
}

void AboutWindow::License()
{
	ImGui::NewLine();
	ImGui::Text("License:");
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::TextWrapped("MIT License \n\nCopyright(c) 2019 ChronoByte \nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), to deal");
	/*	in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions :

	The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.");*/
}
