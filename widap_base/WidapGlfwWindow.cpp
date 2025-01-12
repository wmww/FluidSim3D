#include <GL/glew.h>
#include "WidapGlfwWindow.h"
#include <iostream>

bool WidapGlfwWindow::firstInstance=1, WidapGlfwWindow::error=0;
//char WidapGlfwWindow::key2char[127];

WidapGlfwWindow::WidapGlfwWindow()
{ //constructor
	if (error)
        return;
	
	if (firstInstance)
	{
		if (!glfwInit())
		{
		    error=1;
		    std::cerr << "GLFW failed to init in WidapGlfwWindow constructor\n";
		}
		
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
		
		firstInstance=0;
	}
}

WidapGlfwWindow::~WidapGlfwWindow()
{
    if (windowIsOpen)
        close();
    
    glfwTerminate();
	//windowObj.close();
}

void WidapGlfwWindow::open(int newWdth, int newHght, const char * name)
{
	if (error || windowIsOpen)
        return;
    
    windowObj = glfwCreateWindow(newWdth, newHght, "My Title", NULL, NULL);
	
	if (!windowObj)
    {
        windowIsOpen=0;
        error=1;
        std::cerr << "error opening GLFW window\n";
        return;
    }
	
	windowIsOpen=1;
	
	dim.x=newWdth;
	dim.x=newHght;
	
	glfwMakeContextCurrent(windowObj);
	
	glewInit();
	
	windowHasFocus=1;
	
	windowResize();
	
	update();
}

void WidapGlfwWindow::close()
{
    if (!error && windowIsOpen)
    {
        glfwDestroyWindow(windowObj);
        windowObj=0;
        windowIsOpen=0;
    }
}

void WidapGlfwWindow::clrFill(RGBpix clr, double alpha)
{ //fills the entire window with a single color
	
}

void WidapGlfwWindow::rect(int left, int right, int bottom, int top, RGBpix clr, double alpha)
{ //draws a filled rectangle
	
	GLfloat verts[6];
	
	verts[0]=left;
	verts[1]=bottom;
	
	verts[2]=right;
	verts[3]=bottom;
	
	verts[4]=right;
	verts[5]=top;
	
	for (int i=0; i<6; i+=2)
    {
        verts[i]=grdnt(verts[i], 0, dim.x, 0, 1);
        verts[i+1]=grdnt(verts[i+1], 0, dim.y, 0, 1);
    }
    
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, verts, GL_STREAM_DRAW);
	
glEnableVertexAttribArray(0);

 glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

 glVertexAttribPointer(

    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.

    3,                  // size

    GL_FLOAT,           // type

    GL_FALSE,           // normalized?

    0,                  // stride

    (void*)0            // array buffer offset

 );

 // Draw the triangle !

 glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

 glDisableVertexAttribArray(0);
}

void WidapGlfwWindow::circle(XYint pos, double radius, RGBpix clr, double alpha)
{ //draws a filled circle
	
}

void WidapGlfwWindow::target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha)
{ //draws the corners of a square
	
	int lngth=(right-left+top-bottom)/12;
	
	//upper left:
	rect(left-thickness, left, top-lngth, top, clr, alpha);
	rect(left-thickness, left+lngth, top, top+thickness, clr, alpha);
	
	//upper right:
	rect(right-lngth, right+thickness, top, top+thickness, clr, alpha);
	rect(right, right+thickness, top-lngth, top, clr, alpha);
	
	//lower right:
	rect(right, right+thickness, bottom, bottom+lngth, clr, alpha);
	rect(right-lngth, right+thickness, bottom-thickness, bottom, clr, alpha);
	
	//lower left:
	rect(left-thickness, left, bottom, bottom+lngth, clr, alpha);
	rect(left-thickness, left+lngth, bottom-thickness, bottom, clr, alpha);
}

void WidapGlfwWindow::line(XYint a, XYint b, int thick, RGBpix clr)
{
    
}

void WidapGlfwWindow::rndrText(char txt, double pixHgh, RGBpix clr, double weight)
{
	int lineThick, pixWde; //thickness of the lines and number of pixels each char is wide
	//const double oversize=1.5; //how much we should oversize each char
	//const double horizScale=0.8; //how much to scale horizontally
	
	lineThick=weight*pixHgh/16;
	if (lineThick<1)
		lineThick=1;
	pixWde=pixHgh*0.5;
	
	if (txt=='\n')
	{
		loc.y-=pixHgh*1.5;
		loc.x=locStrt.x;
	}
	else if (txt== '	')
	{
		loc.x=(int)((loc.x-locStrt.x)/(pixWde*6)+1)*6*pixWde+locStrt.x;
	}
	else
	{
		rect(loc.x, loc.x+pixWde, loc.y, loc.y+pixHgh, clr);
		
		loc.x+=pixWde*1.5;
	}
}

void WidapGlfwWindow::trnsfrFromImg(WidapImage* source, Vctr2<int> targetLoc)
{ //transfers an image onto the window
	XYint imgDim;
	int pixCount;
	
	imgDim=source->getDim();
	
	pixCount=imgDim.x*imgDim.y;
	
	if (!pixCount)
		return;
		
    //render the image here
}

void WidapGlfwWindow::display()
{
	
	if (!windowIsOpen)
	{
		//std::cout << ">> update called while window closed <<\n";
		return;
	}
	else if (glfwWindowShouldClose(windowObj))
	{
		close();
		return;
	}
	
	glfwSwapBuffers(windowObj);
}

void WidapGlfwWindow::checkInput()
{
	if (!windowIsOpen)
	{
		//std::cout << ">> update called while window closed <<\n";
		return;
	}
	else if (glfwWindowShouldClose(windowObj))
	{
		close();
		return;
	}
	
	glfwPollEvents();
	
	keyPressListPos=0;
	keyPressNum=0;
	
	mouseLClick=0;
	mouseRClick=0;
	mouseMClick=0;
	
	mouseScroll=0;
	
	/*while (windowObj.pollEvent(event))
	{
		switch (event.type)
		{
		// window closed
		case sf::Event::Closed:
			windowObj.close();
			break;
		
		// key pressed
		case sf::Event::KeyPressed:
			keyPresses[keyPressNum]=key2char[event.key.code];
			++keyPressNum;
			break;
		
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				mouseLClick=1;
				break;
				
			case sf::Mouse::Right:
				mouseRClick=1;
				break;
				
			case sf::Mouse::Middle:
				mouseMClick=1;
				break;
			
			default:
				break;
			}
			break;
		
		case sf::Event::MouseWheelMoved:
			mouseScroll+=event.mouseWheel.delta;
			break;
			
		case sf::Event::Resized:
			windowResize();
			break;
			
		case sf::Event::GainedFocus:
			windowHasFocus=1;
			break;
		
		case sf::Event::LostFocus:
			windowHasFocus=0;
			break;
		
		// we don't process other types of events
		default:
			break;
		}
	}*/
	
	/*sf::Vector2i mouseVctr=sf::Mouse::getPosition();
	//sf::Vector2i windowVctr=windowObj.getPosition();
	
	shiftDwnBool=sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
	ctrlDwnBool=sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
	altDwnBool=sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);
	superDwnBool=sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem) || sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem);
	
	mouseLDwn=sf::Mouse::isButtonPressed(sf::Mouse::Left);
	mouseRDwn=sf::Mouse::isButtonPressed(sf::Mouse::Right);
	mouseMDwn=sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	
	mouseLocDlta.x=mouseVctr.x-windowVctr.x-mouseLocation.x;
	mouseLocDlta.y=dim.y-mouseVctr.y+windowVctr.y-mouseLocation.y;
	
	mouseLocation.x=mouseVctr.x-windowVctr.x;
	mouseLocation.y=dim.y-mouseVctr.y+windowVctr.y;*/
}

void WidapGlfwWindow::windowResize()
{
	//XYint newDim;
	
	//newDim=mkXYint(windowObj.getSize().x, windowObj.getSize().y);
	
	//if (newDim.x==dim.x && newDim.y==dim.y)
		//return;
	
	//dim=newDim;
}



