#pragma once

#include "WidapStd.h"
#include "WidapColors.h"
#include <stdio.h> //for file IO
#include <iostream>
#include <string.h>

//converts an int to bytes (used for writing data to bitmap header)
void num2bytes(unsigned char *ptr, int byteNum, int num);

//converts bytes to an int (used for reading bitmap header)
int bytes2num(unsigned char *ptr, int bytesNum);

struct TextStyle
{
	double height, weight;
	RGBpix color;
};

///image class

class WidapImage
{
public:
	
	struct Glyph
	{
		int lineNum;
		
		XYdbl* lines[2]; // the actual data; the array is for the start and end points of each line
	};
	
	static Glyph widapFont[256];
	
	RGBpix* bits; //a pointer to the start of the image
	
	int getWdth() {return dim.x;} //returns the width
	int getHght() {return dim.y;} //returns the height
	XYint getDim() {return dim;} //returns the dimensions in the form of an XYint
	XYint getLoc() {return loc;}
	XYint getLocStrt() {return locStrt;}
	
	bool deleteBits; //set to one if this object created and thus needs to delete its bits
	
	void setupWidapFont();
	
	WidapImage(int newWdth=0, int newHght=0, RGBpix* bitsPtr=0); //constructor; if bitsPtr is 0 it will make its own bits
	
	~WidapImage(); //destructor
	
	RGBpix* pix(XYint loc); //returns the color of the specified pixel
	void clrFill(RGBpix clr, double alpha=1); //fills the entire image with a single color
	
	void setLoc(int newX, int newY) {setLoc(mkXY(newX, newY));}
	void setLoc(XYint newLoc) {loc=newLoc; locStrt=newLoc;}
	
	void rect(int left, int right, int bottom, int top, RGBpix clr, double alpha=1); //draws a filled rectangle
	void circle(XYint pos, double radius, RGBpix clr, double alpha=1); //draws a filled circle (may be slightly off or look a bit polygony due to rounding, needs to be fixed)
	void triangle(XYint vert0, XYint vert1, XYint vert2, RGBpix clr, double alpha=1); //draws a filled triangle
	void target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha=1); //draws the corners of a square
	void finishPlygn(int thick, RGBpix clr) {line(loc, locStrt, thick, clr); loc=locStrt;} //good line drawing algorithm, alpha not yet implemented
	void line(XYint a, int thick, RGBpix clr) {line(loc, a, thick, clr); loc=a;} //good line drawing algorithm, alpha not yet implemented
	void line(XYint a, XYint b, int thick, RGBpix clr); //good line drawing algorithm, alpha not yet implemented
	void text(const char * txt, XYint newLoc, double pixHgh, RGBpix clr, double weight=1) {setLoc(newLoc); text(txt, pixHgh, clr, weight);} //draws text
	void text(char txt, XYint newLoc, double pixHgh, RGBpix clr, double weight=1) {setLoc(newLoc); text(txt, pixHgh, clr, weight);} //draws text
	void text(const char * txt, TextStyle style) {text(txt, style.height, style.color, style.weight);}
	void text(char txt, TextStyle style) {text(txt, style.height, style.color, style.weight);}
	void text(char txt, double pixHgh, RGBpix clr, double weight=1); //draws text
	void text(const char * txt, double pixHgh, RGBpix clr, double weight=1); //draws text
	
	//void setSize(int newHght, int newWdth); //not yet implemented; sets the dimensions of the image (NOTE: does not do anything with the data)
	void setBits(RGBpix* bitsPtr=0) {setBits(dim, bitsPtr);} //sets a new pointer for the image but does not change the image dimensions
	void setBits(XYint newDim, RGBpix* bitsPtr=0); //sets a new pointer for the image (if zero does not actually make an image, you need to call load or newImage for that)
	void newImage(XYint newDim); //makes a new image of the specified size but does not fill it with any color
	bool load(const char* filename); //loads a single bitmap into the image
	void trnsfr(WidapImage* othrImg, int x=0, int y=0); //transfers this image to another
	void save(const char* filename); //saves the image to a bitmap
	
private:
	
	XYint dim; //the dimensions of the bitmap
	XYint loc, locStrt; //used for where to draw text and polygons
	int pixNum; //total number of pixels (wdth*hght)
	static bool init; //if to init
};


