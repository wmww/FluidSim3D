#pragma once

#include "WidapStd.h"
#include "WidapImage.h"
#include "Vctr2.h"

class WidapWindowBase
{
public:
	
	WidapWindowBase();
	//WidapWindowBase(int newWdth, int newHght, const char * name); //if dimensions are 0 it will be full screen
	virtual ~WidapWindowBase();
	
	int getWdth() {return dim.x;} //returns the width
	int getHght() {return dim.y;} //returns the height
	Vctr2<int> getDim() {return dim;} //returns the dimensions in the form of an XYint
	Vctr2<int> getLoc() {return loc;}
	Vctr2<int> getLocStrt() {return locStrt;}
	
	char nextKey();	//get the next key that was pressed
		//0 is returned once when the end of the list is reached, then it starts over; you can cycle through as many times as you want
		//if a cycle goes by without calling this then the key presses of that cycle are NOT saved
	
	char lastKey(); //returns the most resent key pressed or 0 if no key was pressed last cycle
	
	//get if the modifiers are down
	bool shiftDwn() {return shiftDwnBool;};
	bool ctrlDwn() {return ctrlDwnBool;};
	bool altDwn() {return altDwnBool;};
	bool superDwn() {return superDwnBool;};
	
	Vctr2<int> mouseLoc() {return mouseLocation;}
	Vctr2<int> mouseDlta() {return mouseLocDlta;}
	
	bool lClick() {return mouseLClick;}
	bool rClick() {return mouseRClick;}
	bool mClick() {return mouseMClick;}
	
	bool lDwn() {return mouseLDwn;}
	bool rDwn() {return mouseRDwn;}
	bool mDwn() {return mouseMDwn;}
	
	int scroll() {return mouseScroll;}
	
	bool hasFocus() {return windowHasFocus;}
	bool isOpen() {return windowIsOpen;}
	
	void setLoc(Vctr2<int> newLoc) {loc=newLoc; locStrt=newLoc;};
	
	virtual void open(int newWdth, int newHght, const char * name)=0;
	
	virtual void clrFill(RGBpix clr, double alpha=1)=0; //fills the entire image with a single color
	virtual void rect(int left, int right, int bottom, int top, RGBpix clr, double alpha=1)=0; //draws a filled rectangle
	virtual void circle(XYint pos, double radius, RGBpix clr, double alpha=1)=0; //draws a filled circle (may be slightly off or look a bit polygony due to rounding, needs to be fixed)
	virtual void target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha=1)=0; //draws the corners of a square
	void finishPlygn(int thick, RGBpix clr) {line(loc, locStrt, thick, clr); loc=locStrt;} //good line drawing algorithm, alpha not yet implemented
	void line(XYint a, int thick, RGBpix clr) {line(loc, a, thick, clr); loc=a;} //good line drawing algorithm, alpha not yet implemented
	virtual void line(XYint a, XYint b, int thick, RGBpix clr)=0; //good line drawing algorithm, alpha not yet implemented
	void text(const char * txt, XYint newLoc, double pixHgh, RGBpix clr, double weight=1) {setLoc(newLoc); text(txt, pixHgh, clr, weight);} //draws text
	void text(char txt, XYint newLoc, double pixHgh, RGBpix clr, double weight=1) {setLoc(newLoc); text(txt, pixHgh, clr, weight);} //draws text
	void text(const char * txt, TextStyle style) {text(txt, style.height, style.color, style.weight);}
	void text(char txt, TextStyle style) {text(txt, style.height, style.color, style.weight);}
	void text(const char * txt, double pixHgh, RGBpix clr, double weight=1); //draws text
	void text(char txt, double pixHgh, RGBpix clr, double weight=1) {rndrText(txt, pixHgh, clr, weight=1);}; //draws text
	virtual void rndrText(char txt, double pixHgh, RGBpix clr, double weight=1)=0; //draws text
	
	virtual void trnsfrFromImg(WidapImage* othrImg, Vctr2<int> loc)=0; //transfers an image onto this window
	
	void update() {display(); checkInput();}; //calls display and then checkInput
	virtual void display()=0;
	virtual void checkInput()=0;
	virtual void close()=0;
	
protected:
	Vctr2<int> dim; //the dimensions of the bitmap
	Vctr2<int> loc, locStrt; //used for where to draw text and polygons
	
	Vctr2<int> mouseLocation; //the location of the mouse
	Vctr2<int> mouseLocDlta; //how much the mouse has moved
	bool mouseLClick, mouseRClick, mouseMClick; //if left, right or middle clicks have happened last cycle
	bool mouseLDwn, mouseRDwn, mouseMDwn; //if these buttons are down
	int mouseScroll; //can be positive, negative or 0, how much the scroll wheel has moved
	
	bool shiftDwnBool, ctrlDwnBool, altDwnBool, superDwnBool;
	
	char keyPresses[64]; //a list of key presses
	int keyPressNum; //how long the above list is
	int keyPressListPos; //cycles as nextKey is called
	
	bool windowHasFocus;
	bool windowIsOpen;
};
