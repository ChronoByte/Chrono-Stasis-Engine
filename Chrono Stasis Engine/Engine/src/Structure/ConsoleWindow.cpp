#include "ConsoleWindow.h"
#include "csApp.h"

#include <string>

ConsoleWindow::ConsoleWindow(bool startOpened) : Window(startOpened)
{	
	active = true; 
}


ConsoleWindow::~ConsoleWindow()
{
	Clear(); 
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

	ImGui::SameLine();
	if (ImGui::Button("Push Success"))
		PushLog("Here we are pushing a success to the console");
	
	ImGui::SameLine();
	if (ImGui::Button("Push Separator"))
		PushLog("[-----------------------------------------------------------]");

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	PrintAllLogs();

	if (scrolltobottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(0.0f);

	scrolltobottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();

	if (ImGui::Button("Clear")) 
		Clear(); 

	ImGui::SameLine();

	if (ImGui::Button("Go To Bottom")) 
		scrolltobottom = true;

	ImGui::End();
}

void ConsoleWindow::PrintAllLogs()
{
	for (int i = 0; i < loglist.size(); i++)
	{
		const char* item = loglist[i];

		// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
		bool pop_color = false;
		if (strstr(item, "error") || strstr(item, "Error")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
		if (strstr(item, "warning") || strstr(item, "Warning")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); pop_color = true; }
		if (strstr(item, "success") || strstr(item, "Success")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); pop_color = true; }
		ImGui::TextUnformatted(item);
		if (pop_color)
			ImGui::PopStyleColor();
	}
}

void ConsoleWindow::PushLog(char * log)
{
	loglist.push_back(strdup(log));
}

void ConsoleWindow::Clear()
{
	for (int i = 0; i < loglist.size(); i++)
		free(loglist[i]);

	loglist.clear(); 
}
