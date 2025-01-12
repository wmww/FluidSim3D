#include "WidapSfmlWindow.h"
#include <iostream>

bool WidapSfmlWindow::firstInstance=1;
sf::Font WidapSfmlWindow::font;
char WidapSfmlWindow::key2char[127];

/*
-lsfml-window
-lsfml-graphics
-lsfml-system
^^^ must add this to linker settings
*/

WidapSfmlWindow::WidapSfmlWindow()
{ //constructor
	
	dataRGBA=0;
	
	if (firstInstance)
	{
		if
		(
			!font.loadFromFile("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf") &&
			!font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf") &&
			!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf")
		)
		{
			std::cout << ">> font failed to load <<\n";
		}
		
		int i;

		for (i=0; i<127; ++i)
		{
			key2char[i]=1;
		}

		for (i=sf::Keyboard::A; i<=sf::Keyboard::Z; ++i) //letters
			key2char[i]=i-sf::Keyboard::A+int('A');

		for (i=sf::Keyboard::Num0; i<=sf::Keyboard::Num9; ++i) //letters
			key2char[i]=i-sf::Keyboard::Num0+int('0');

		for (i=sf::Keyboard::Numpad0; i<=sf::Keyboard::Numpad9; ++i) //letters
			key2char[i]=i-sf::Keyboard::Numpad0+int('0');

		key2char[int(sf::Keyboard::Space)]=' ';
		key2char[int(sf::Keyboard::Period)]='.';
		key2char[int(sf::Keyboard::Comma)]=',';
		key2char[int(sf::Keyboard::BackSlash)]='\\';
		key2char[int(sf::Keyboard::Slash)]='/';
		key2char[int(sf::Keyboard::LBracket)]='[';
		key2char[int(sf::Keyboard::RBracket)]=']';
		key2char[int(sf::Keyboard::Quote)]='\'';
		key2char[int(sf::Keyboard::SemiColon)]=';';
		key2char[int(sf::Keyboard::Delete)]=127;
		key2char[int(sf::Keyboard::BackSpace)]='\b';
		key2char[int(sf::Keyboard::Return)]='\n';
		key2char[int(sf::Keyboard::Tab)]='	';
		key2char[int(sf::Keyboard::Equal)]='=';
		key2char[int(sf::Keyboard::Dash)]='-';
		key2char[int(sf::Keyboard::Left)]='a'; //I use lower key2char[int(wasd for arrow keys
		key2char[int(sf::Keyboard::Right)]='d';
		key2char[int(sf::Keyboard::Up)]='w';
		key2char[int(sf::Keyboard::Down)]='s';
		
		firstInstance=0;
	}
}

/*
WidapSfmlWindow::WidapSfmlWindow(int newWdth, int newHght, const char * name)
{ //constructor
	
	WidapSfmlWindow();
	open(newWdth, newHght, name);
}*/

WidapSfmlWindow::~WidapSfmlWindow()
{
	if (dataRGBA)
		delete[] dataRGBA;
	
	windowObj.close();
}

void WidapSfmlWindow::open(int newWdth, int newHght, const char * name)
{
	if (newWdth && newHght)
		windowObj.create(sf::VideoMode(newWdth, newHght), name);
	else
		windowObj.create(sf::VideoMode::getDesktopMode(), name, sf::Style::Fullscreen);
	
	windowResize();
	
	windowIsOpen=windowObj.isOpen();
	
	windowHasFocus=windowIsOpen;
	
	update();
}

void WidapSfmlWindow::clrFill(RGBpix clr, double alpha)
{ //fills the entire window with a single color
	
	if (alpha==1)
		windowObj.clear(sf::Color(clr.r, clr.g, clr.b, 255));
	else
		rect(0, dim.x, 0, dim.y, clr, alpha);
}

void WidapSfmlWindow::rect(int left, int right, int bottom, int top, RGBpix clr, double alpha)
{ //draws a filled rectangle
	
	sf::RectangleShape rectangle(sf::Vector2f(right-left, top-bottom));
	rectangle.setFillColor(sf::Color(clr.r, clr.g, clr.b, alpha*255));
	rectangle.setPosition(left, dim.y-top);
	
	windowObj.draw(rectangle);
}

void WidapSfmlWindow::circle(XYint pos, double radius, RGBpix clr, double alpha)
{ //draws a filled circle
	
	sf::CircleShape shape(radius);
	shape.setFillColor(sf::Color(clr.r, clr.g, clr.b, alpha*255));
	shape.setPosition(pos.x-radius, dim.y-pos.y-radius);
	
	windowObj.draw(shape);
}

void WidapSfmlWindow::target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha)
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

void WidapSfmlWindow::line(XYint a, XYint b, int thick, RGBpix clr)
{
	double rot=-atan2(b.y-a.y, b.x-a.x);
	sf::RectangleShape rectangle(sf::Vector2f(dst(a, b), thick));
	rectangle.setFillColor(sf::Color(clr.r, clr.g, clr.b, 255));
	rectangle.setPosition(a.x+sin(rot)*thick/2, dim.y-a.y-cos(rot)*thick/2);
	rectangle.setRotation(rad2deg(rot));
	
	windowObj.draw(rectangle);
	circle(a, thick/2, clr);
	circle(b, thick/2, clr);
	//circle(a, thick/2, ::clr(0, 0, 0), 0.5);
	//circle(b, thick/2, ::clr(0, 0, 0), 0.5);
}

void WidapSfmlWindow::rndrText(char txt, double pixHgh, RGBpix clr, double weight)
{
	int lineThick, pixWde; //thickness of the lines and number of pixels each char is wide
	const double oversize=1.5; //how much we should oversize each char
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
		sf::Text text(txt, font, pixHgh);
		text.setPosition(loc.x, dim.y-loc.y-pixHgh*oversize);
		text.setColor(sf::Color(clr.r, clr.g, clr.b, 255));
		if (weight>1.9)
			text.setStyle(sf::Text::Bold);
		text.setCharacterSize(pixHgh*oversize);
		//text.setScale(sf::Vector2f(horizScale, 1));
		
		windowObj.draw(text);
		
		loc.x+=pixWde*1.5;
	}
}

void WidapSfmlWindow::trnsfrFromImg(WidapImage* source, Vctr2<int> targetLoc)
{ //transfers an image onto the window
	XYint imgDim;
	int pixCount;
	
	imgDim=source->getDim();
	
	pixCount=imgDim.x*imgDim.y;
	
	if (!pixCount)
		return;
	else if (!dataRGBA || imgDim.x>(int)texture.getSize().x || imgDim.y>(int)texture.getSize().y)
	{
		if (dataRGBA)
			delete[] dataRGBA;
		
		if (!(dataRGBA=new RGBApix[pixCount]))
		{
			std::cout << ">> memory error in WidapSfmlWindow <<\n";
			return;
		}
		
		if (!texture.create(imgDim.x, imgDim.y));
	}
	
	int x, y, yFctr0, yFctr1;
	RGBpix color;
	
	for (y=0; y<imgDim.y; ++y)
	{
		yFctr0=y*imgDim.x;
		yFctr1=(imgDim.y-y-1)*imgDim.x;
		for (x=0; x<imgDim.x; ++x)
		{
			color=source->bits[x+yFctr0];
			
			dataRGBA[yFctr1+x].r=color.r;
			dataRGBA[yFctr1+x].g=color.g;
			dataRGBA[yFctr1+x].b=color.b;
			dataRGBA[yFctr1+x].a=255;
		}
	}
	
	texture.update((sf::Uint8*)dataRGBA, imgDim.x, imgDim.y, 0, 0);
	
	sf::Sprite sprite(texture);
	
	sprite.setPosition(sf::Vector2f(targetLoc.x, dim.y-targetLoc.y-imgDim.y));
	
	windowObj.draw(sprite);
}

void WidapSfmlWindow::display()
{
	if (!windowObj.isOpen())
	{
		windowIsOpen=0;
		return;
	}
	else if (!windowIsOpen)
	{
		//std::cout << ">> update called while window closed <<\n";
		return;
	}
	
	windowObj.display();
}

void WidapSfmlWindow::checkInput()
{
	if (!windowObj.isOpen())
	{
		windowIsOpen=0;
		return;
	}
	else if (!windowIsOpen)
	{
		//std::cout << ">> update called while window closed <<\n";
		return;
	}
	
	sf::Event event;
	
	keyPressListPos=0;
	keyPressNum=0;
	
	mouseLClick=0;
	mouseRClick=0;
	mouseMClick=0;
	
	mouseScroll=0;
	
	while (windowObj.pollEvent(event))
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
	}
	
	sf::Vector2i mouseVctr=sf::Mouse::getPosition();
	sf::Vector2i windowVctr=windowObj.getPosition();
	
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
	mouseLocation.y=dim.y-mouseVctr.y+windowVctr.y;
}

void WidapSfmlWindow::windowResize()
{
	XYint newDim;
	
	newDim=mkXYint(windowObj.getSize().x, windowObj.getSize().y);
	
	if (newDim.x==dim.x && newDim.y==dim.y)
		return;
	
	dim=newDim;
	
	//stop view from scaling with the window
	
	windowObj.setView(sf::View(sf::FloatRect(0, 0, windowObj.getSize().x, windowObj.getSize().y)));
	
	//dspWndw.clrFill(clrBknd);
}
