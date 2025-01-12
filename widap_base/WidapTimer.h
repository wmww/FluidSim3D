#pragma once

#include <iostream> //needed for sleeping and timing, must add -lrt to linker settings
//#include <stdlib.h>
#include <unistd.h> //used for sleeping
//#include <sys/time.h> // must add -lrt to linker settings to work

class WidapTimer
{
public:
	WidapTimer();
	~WidapTimer();
	
	void waitUntil(double timeToWait, bool resetAfter=1); //waits until a specific time, then updates current time
	void waitFor(double timeToWait, bool resetAfter=0); //waits for a given amount of time
	double checkTime(); //updates and returns time as a double
	
	void update(bool reset=0); //updates lastCheck to the current time
	
private:
	timespec baseTime;
	timespec lastCheck;
};
