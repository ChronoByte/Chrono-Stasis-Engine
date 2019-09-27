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
	uint32 Read() const;
	float ReadSec() const;

private:

	uint32	started_at = 0;
	
};

#endif //__TIMER_H__