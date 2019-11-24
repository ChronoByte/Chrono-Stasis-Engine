// ----------------------------------------------------
// ctTimer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
Timer::Timer()
{
}

// ---------------------------------------------
void Timer::Start()
{
	
	started_at = SDL_GetTicks();
	isPaused = false; 
}

void Timer::Stop()
{
	started_at = 0;
	paused_at = 0; 
	isPaused = true; 
	
}

// ---------------------------------------------

uint32 Timer::Read() const
{
	return isPaused ? paused_at : SDL_GetTicks() - started_at;
}

// ---------------------------------------------

void Timer::Pause()
{
	if (isPaused)
		return; 

	paused_at = SDL_GetTicks() - started_at;
	isPaused = true; 
	
}

// ---------------------------------------------

void Timer::Resume()
{
	if (!isPaused)
		return; 

	started_at = SDL_GetTicks() - paused_at;
	isPaused = false; 
	
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return isPaused ? float(paused_at / 1000.0f) : float(SDL_GetTicks() - started_at) / 1000.0f;
}


