#include "WidapWindowBase.h"
#include <iostream>

WidapWindowBase::WidapWindowBase()
{
	mouseLClick=0;
	mouseRClick=0;
	mouseMClick=0;
	
	mouseLDwn=0;
	mouseRDwn=0;
	mouseMDwn=0;
	mouseScroll=0;
	
	shiftDwnBool=0;
	ctrlDwnBool=0;
	altDwnBool=0;
	superDwnBool=0;
	
	keyPressNum=0;
	keyPressListPos=0;
	
	windowHasFocus=0;
	windowIsOpen=0;
	
	setLoc(Vctr2<int>(0, 0));
}

/*WidapWindowBase::WidapWindowBase(int newWdth, int newHght, const char * name)
{
	WidapWindowBase();
	open(newWdth, newHght, name);
}*/

WidapWindowBase::~WidapWindowBase()
{
	
}

char WidapWindowBase::nextKey()
{
	if (keyPressListPos<keyPressNum)
	{
		++keyPressListPos;
		return keyPresses[keyPressListPos-1];
	}
	else
	{
		keyPressListPos=0;
		return 0;
	}
}

char WidapWindowBase::lastKey()
{
	if (keyPressNum)
	{
		return keyPresses[keyPressNum-1];
	}
	else
	{
		return 0;
	}
}

void WidapWindowBase::text(const char * txt, double pixHgh, RGBpix clr, double weight)
{ //draws text
	
	int i=0;
	
	const int MAX_LNG=2048; //max text length, stops non terminated strings from crashing everything
	
	while (txt[i]!=0 && i<MAX_LNG)
	{
		rndrText(txt[i], pixHgh, clr, weight);
		
		++i;
	}
	if (i==MAX_LNG)
		std::cout << ">> string sent to WidapWindowBase::text() was not terminated before position " << MAX_LNG << "<<\n";
}


