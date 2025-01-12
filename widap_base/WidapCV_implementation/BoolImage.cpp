
#include "../WidapCV.h"

BoolImage::BoolImage()
{
	wdth=0;
	hght=0;
	bits=0;
}

BoolImage::~BoolImage()
{
	if (bits)
		delete[] bits;
}

void BoolImage::hslRange(WidapImage* img, HSLpix low, HSLpix hgh)
{
	int i, pixNum;
	HSLpix hsl;
	
	newImage(img->getWdth(), img->getHght());
	
	pixNum=wdth*hght;
	
	for (i=0; i<pixNum; ++i)
	{
		hsl=rgb2hsl(*(img->bits+i));
		
		*(bits+i)=((hsl.l>=low.l && hsl.l<=hgh.l) && (hsl.l<32 || hsl.l>480 || (hsl.s>=low.s && hsl.s<=hgh.s && (hsl.s<16 || ((hsl.h>=low.h && hsl.h<hgh.h)==(low.h<hgh.h))))));
	}
}

void BoolImage::rgbDlta(WidapImage* img, int thresh)
{
	int i, pixNum;
	
	newImage(img->getWdth(), img->getHght());
	
	pixNum=wdth*hght;
	
	for (i=0; i<wdth; ++i)
		*(bits+i)=0;
	
	for (i=0; i<hght; ++i)
		*(bits+i*wdth)=0;
	
	for (i=wdth+1; (i%wdth!=0 || ++i<pixNum); ++i)
	{
		*(bits+i)=(std::max(clrDif(*(img->bits+i), *(img->bits+i-1)), clrDif(*(img->bits+i), *(img->bits+i-wdth)))>=thresh);
	}
}

bool* BoolImage::pix(unsigned int x, unsigned int y)
{ //returns a pointer to the specified pixel
	if (x>=(unsigned int)wdth || y>=(unsigned int)hght)
	{
		std::cout << std::endl << ">> tried to access outside of image <<" << std::endl;
		return (bits+clamp((int)x, 0, wdth)+clamp((int)y, 0, hght)*wdth);
	}
	else
	{
		return (bits+x+y*wdth);
	}
}

void BoolImage::newImage(int newWdth, int newHgth)
{ //makes a new image of the specified size but does not fill it with any color
	
	if (bits)
		delete[] bits;
	
	wdth=newWdth;
	hght=newHgth;
	bits=new bool[wdth*hght];
}

void BoolImage::trnsfr(WidapImage* othrImg, RGBpix clr0, RGBpix clr1)
{ //transfers this image onto another one; x and y specify the lower left corner
	
	if (!bits)
	{
		std::cout << ">> tried to transfer, but bool image empty <<" << std::endl;
		return;
	}
	
	if (!othrImg->bits)
		othrImg->newImage(mkXY(wdth, hght));
	
	int xMax=clamp(wdth, 0, othrImg->getWdth());
	int yMax=clamp(hght, 0, othrImg->getHght());
	
	int othrImgWdth=othrImg->getWdth();
	
	int x, y;
	int yFactor, yFactor1;
	
	for (y=0; y<yMax; ++y)
	{
		yFactor=y*othrImgWdth;
		yFactor1=y*wdth;
		
		for (x=0; x<xMax; ++x)
		{
			if (bits[x+yFactor1])
				othrImg->bits[x+yFactor]=clr1;
			else
				othrImg->bits[x+yFactor]=clr0;
		}
	}
}
