#include "ConsoleWindow.h"
#include "csApp.h"

ConsoleWindow::ConsoleWindow(Application* app) : Window(app)
{	
	active = true; 
}


ConsoleWindow::~ConsoleWindow()
{
}


void ConsoleWindow::Draw()
{
	
	ImGui::Begin("Console", &active);

	if (ImGui::Button("Push Log"))
		PushLog("Here we are pushing a standard log to the console");

	ImGui::SameLine();
	if (ImGui::Button("Push Error"))
		PushLog("Here we are pushing an error to the console");

	ImGui::SameLine();
	if (ImGui::Button("Push Warning"))
		PushLog("Here we are pushing a warning to the console");

	ImGui::Checkbox("Auto-scroll", &autoscroll);

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < oplist.size(); i++)
	{
		const char* item = oplist[i];

		// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
		bool pop_color = false;
		if (strstr(item, "error")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
		if (strstr(item, "warning")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); pop_color = true; }
		ImGui::TextUnformatted(item);
		if (pop_color)
			ImGui::PopStyleColor();
	}

	if (scrolltobottom || (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(0.0f);

	scrolltobottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();

	if (ImGui::Button("Clear")) { Clear(); }

	ImGui::End();
}

void ConsoleWindow::PushLog(char * log)
{
	oplist.push_back(log);
}

void ConsoleWindow::Clear()
{
	oplist.clear(); 
}
