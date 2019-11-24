#ifndef __TIMER_H__
#define __TIMER_H__

#include "csGlobals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();

	void Stop(); 

	void Pause(); 
	void Resume(); 

	uint32 Read() const;
	float ReadSec() const;
	
private:

	uint32 paused_at = 0;
	uint32	started_at = 0;
	bool isPaused = true; 
	
};

#endif //__TIMER_H__