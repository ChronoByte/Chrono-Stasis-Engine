#include "TimeWindow.h"
#include "csApp.h"
#include "GameViewWindow.h"

TimeWindow::TimeWindow(bool startOpened) : Window(startOpened)
{
}

TimeWindow::~TimeWindow()
{
}

void TimeWindow::Draw()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;

	if (ImGui::Begin("Game Logic", nullptr, flags))
	{
		if (App->gameState == GameState::ONSTOP)
		{
			DisplayOnStopped(); 
		}
		else
		{
			DisplayOnPlay();
		}

		ImGui::SameLine();
		ImGui::Checkbox("Options", &showOptions); 

		if (showOptions)
		{
			ShowOptions();
		}
	
	}
	ImGui::End();

}

void TimeWindow::ShowOptions()
{
	ImGui::Text("Time since start: %.2f", (float)App->gameTimer.ReadSec());
	ImGui::SliderFloat("Delta Time Multiplier", &App->dtMultiplier, 0.001f, MAX_DT_MULTIPLIER);
	ImGui::SameLine();
	if (ImGui::SmallButton("Reset"))
		App->dtMultiplier = 1.f;
}

void TimeWindow::DisplayOnStopped()
{
	PlayButton();
	ImGui::SameLine();
	
	PauseButton();
	ImGui::SameLine();
	
	StepButton();
}

void TimeWindow::DisplayOnPlay()
{
	ImGui::SameLine();

	StopButton();
	ImGui::SameLine();

	if (App->gameState == GameState::ONPAUSE)
		ResumeButton();
	
	else if(App->gameState == GameState::ONPLAY)
		PauseButton(); 
	
	ImGui::SameLine();

	StepButton(); 

}

void TimeWindow::StopButton()
{
	if (ImGui::Button("STOP"))
	{
		// Set game window back at how it was before play
		App->editor->gameView->SetActive(wasGameOpen); 

		App->SetGameState(GameState::ONSTOP); 
	}
}


void TimeWindow::ResumeButton()
{
	if (ImGui::Button("RESUME"))
	{
		App->gameTimer.Resume();
		App->gameState = GameState::ONPLAY;
	}
}

void TimeWindow::PlayButton()
{
	if (ImGui::Button("PLAY"))
	{
		TryToOpenGameWindow();
		App->SetGameState(GameState::ONPLAY);
	}
}

void TimeWindow::StepButton()
{
	if (ImGui::Button("STEP"))
	{
		TryToOpenGameWindow();
		App->toDoStep = true; 

		// If its on stop, start the game
		if (App->gameState == GameState::ONSTOP)
			App->SetGameState(GameState::ONPLAY);

		else // Its either on play or pause
		{
			App->gameState = GameState::ONPLAY;
			App->gameTimer.Resume();
		}
	}
}

void TimeWindow::TryToOpenGameWindow()
{
	wasGameOpen = App->editor->gameView->GetActive();
	App->editor->gameView->SetActive(true);
}

void TimeWindow::PauseButton()
{
	if (ImGui::Button("PAUSE"))
	{
		if (App->gameState == GameState::ONSTOP)
			return; 

		App->SetGameState(GameState::ONPAUSE);
	}
}





