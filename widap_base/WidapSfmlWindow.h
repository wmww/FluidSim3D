#pragma once

#include "WidapWindowBase.h"
#include <SFML/Graphics.hpp>

/**linker settings
-lsfml-graphics
-lsfml-window
-lsfml-system
**/

class WidapSfmlWindow : public WidapWindowBase
{
public:
	WidapSfmlWindow(); //constructor
	~WidapSfmlWindow(); //destructor
	
	void open(int newWdth, int newHght, const char * name);
	
	void clrFill(RGBpix clr, double alpha=1); //fills the entire image with a single color
	void rect(int left, int right, int bottom, int top, RGBpix clr, double alpha=1); //draws a filled rectangle
	void circle(XYint pos, double radius, RGBpix clr, double alpha=1); //draws a filled circle (may be slightly off or look a bit polygony due to rounding, needs to be fixed)
	void target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha=1); //draws the corners of a square
	void line(XYint a, XYint b, int thick, RGBpix clr); //good line drawing algorithm, alpha not yet implemented
	void rndrText(char txt, double pixHgh, RGBpix clr, double weight=1); //draws text; not simply called text() due to difficulty overloading virtual functions
	
	void trnsfrFromImg(WidapImage* othrImg, Vctr2<int> loc); //transfers this image to another
	
	void display();
	void checkInput();
	void close() {windowObj.close();}
	
	sf::RenderWindow * getWindowObj() {return &windowObj;}
	
private:
	sf::RenderWindow windowObj;
	
	static bool firstInstance;
	static sf::Font font;
	static char key2char[127];
	
	RGBApix *dataRGBA;
	sf::Texture texture;
	
	void windowResize();
};
