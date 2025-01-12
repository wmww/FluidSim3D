#include "WidapTimer.h"

WidapTimer::WidapTimer()
{
	/** | | | */
	/** V V V */ clock_gettime(CLOCK_REALTIME, &baseTime);
	///must add -lrt to linker options for clock_gettime to work
}

WidapTimer::~WidapTimer()
{

}

void WidapTimer::waitUntil(double timeToWait, bool resetAfter)
{
	double timeDif;

	timeDif=checkTime();

	if (timeDif<timeToWait)
		usleep((timeToWait-timeDif)*1000000); //microseconds
	
	update();
	
	if (resetAfter)
		baseTime=lastCheck;
	
	//vv this would keep the timer from slowly slipping when reset but there is a glitch with partial seconds so don't enable vv
	//lastCheck.tv_sec=baseTime.tv_sec+(int)timeToWait;
	//lastCheck.tv_nsec=(baseTime.tv_nsec+int((timeToWait-(int)timeToWait)*1000000000))%1000000000; //nanoseconds
}

void WidapTimer::waitFor(double timeToWait, bool resetAfter)
{
	usleep(timeToWait*1000000);
	
	update();
	
	if (resetAfter)
		baseTime=lastCheck;
}

void WidapTimer::update(bool reset)
{
	clock_gettime(CLOCK_REALTIME, &lastCheck);
	
	if (reset)
		baseTime=lastCheck;
}

double WidapTimer::checkTime()
{
	double time;
	
	update(0);
	
	time=lastCheck.tv_sec-baseTime.tv_sec+(lastCheck.tv_nsec-baseTime.tv_nsec)/1000000000.0;
	
	return time;
}


