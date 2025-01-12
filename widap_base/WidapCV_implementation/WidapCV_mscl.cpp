#include "../WidapCV.h"

bool trigCached=0; //used to determine if the trig caches have been made (should only be changed by cacheTrig())
double sinCache[TRIG_CACHE_LNG], cosCache[TRIG_CACHE_LNG];

void cacheTrig()
{ //this must be called at the beginning of the program or things that rely on the trig caches will break
	int i;
	double pos;
	
	if (trigCached)
	{
		std::cout << ">> tried to call cacheTrig() multiple times <<" << std::endl;
		return;
	}
	
	std::cout << "calculating trig caches...";
	
	for (i=0; i<TRIG_CACHE_LNG; ++i)
	{
		pos=grdnt(i, 0, TRIG_CACHE_LNG, 0, 2*PI);
		sinCache[i]=sin(pos);
		cosCache[i]=cos(pos);
	}
	
	trigCached=1;
	
	std::cout << "done.\n";
}

