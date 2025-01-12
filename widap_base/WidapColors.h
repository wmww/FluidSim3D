#pragma once

#include "WidapStd.h"

///color structs

struct RGBpix
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

struct RGBApix
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct HSLpix
{
	int h; //hue; from 0 to 1530 (255*6); is the color part
	int s; //saturation; from 0 to 255; the distance between the lowest and highest RGB value; how vivid the color is
	int l; //lightness; 0 to 510; the lowest RGB value added to the highest
};

///color functions

inline RGBpix clr(unsigned char r, unsigned char g, unsigned char b);

inline RGBApix clr(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

inline HSLpix clrHSL(int h, int s, int l);

inline RGBpix rgba2rgb(RGBApix inpt);

inline RGBApix rgb2rgba(RGBpix inpt);

inline HSLpix rgb2hsl(RGBpix rgb);

inline RGBpix hsl2rgb(HSLpix hsl);

inline int clrDif(RGBpix clr0, RGBpix clr1); //returns the difference between two RGB colors (sum of the squares of the differences of the channels)

inline void blend(RGBpix* clr1, RGBpix clr2, double blendFactor);

inline void cmpst(RGBpix *clr1, RGBApix clr2); //composts colors using alpha value


///inline functions

inline RGBpix clr(unsigned char r, unsigned char g, unsigned char b)
{
	RGBpix otpt;
	
	otpt.r=r;
	otpt.g=g;
	otpt.b=b;
	
	return otpt;
}

inline RGBApix clr(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	RGBApix otpt;
	
	otpt.r=r;
	otpt.g=g;
	otpt.b=b;
	otpt.a=a;
	
	return otpt;
}

inline HSLpix clrHSL(int h, int s, int l)
{
	HSLpix hsl;
	
	hsl.h=h;
	hsl.s=s;
	hsl.l=l;
	
	return hsl;
}

inline RGBpix rgba2rgb(RGBApix inpt)
{
	RGBpix otpt;
	
	otpt.r=inpt.r;
	otpt.g=inpt.g;
	otpt.g=inpt.g;
	
	return otpt;
}

inline RGBApix rgb2rgba(RGBpix inpt)
{
	RGBApix otpt;
	
	otpt.r=inpt.r;
	otpt.g=inpt.g;
	otpt.g=inpt.g;
	otpt.a=255;
	
	return otpt;
}

inline HSLpix rgb2hsl(RGBpix rgb)
{
	HSLpix hsl;
	
	unsigned char *hgh, *mid, *low; //pointers that will be set to the 3 RGB components (hgh to the one with the heighest value etc.)
	
	int range; //from 0 to 5; the general color; the heigh, mid and low color components can be detirmened from range
	
	//tree format to make it run faster:
	if (rgb.g<rgb.b) {
		if (rgb.g>rgb.r) {
			range=3;
			hgh=&rgb.b;
			mid=&rgb.g;
			low=&rgb.r;
		} else if (rgb.b>rgb.r) {
			range=4;
			hgh=&rgb.b;
			mid=&rgb.r;
			low=&rgb.g;
		} else {
			range=5;
			hgh=&rgb.r;
			mid=&rgb.b;
			low=&rgb.g;
		}
	} else {
		if (rgb.r>rgb.g) {
			range=0;
			hgh=&rgb.r;
			mid=&rgb.g;
			low=&rgb.b;
		} else if (rgb.r>rgb.b) {
			range=1;
			hgh=&rgb.g;
			mid=&rgb.r;
			low=&rgb.b;
		} else {
			range=2;
			hgh=&rgb.g;
			mid=&rgb.b;
			low=&rgb.r;
		}
	}
	
	//this is a slightly slower but cooler way of setting range:
	//range=(rgb.g<rgb.b)*3+1+((rgb.r>rgb.g)-(rgb.b>rgb.r))*(1-(rgb.g>rgb.b)*2);
	
	//this is a slightly slower but cooler way of setting hgh, mid and low:
	/*hgh=&rgb.b+((22+range*5)/2)%3;
	mdl=&rgb.b+((19+range))%3;
	low=&rgb.b+((19+range*5)/2)%3;*/
	
	hsl.s=(*hgh)-(*low);
	
	if (*low==*hgh)
		hsl.h=0;
	else if (range%2)
		hsl.h=range*255+255-((*mid)-(*low))*255/(hsl.s); //this is basicly grdnt(...) but may be a bit faster
	else
		hsl.h=range*255+((*mid)-(*low))*255/(hsl.s);
	
	hsl.l=(*hgh)+(*low);
	
	return hsl;
}

inline RGBpix hsl2rgb(HSLpix hsl)
{
	RGBpix rgb;
	
	//fix potential bad values:
	while (hsl.h<0)
		hsl.h+=1530;
	while (hsl.h>=1530)
		hsl.h-=1530;
	
	hsl.l=clamp(hsl.l, 0, 510);
	hsl.s=clamp(hsl.s, 0, 255-abs(hsl.l-255));
	
	int range=hsl.h/255; //from 0 to 5; the general color; the heigh, mid and low color components can be detirmened from range
	
	unsigned char *hgh, *mid, *low; //pointers that will be set to the 3 RGB components (hgh to the one with the heighest value etc.)
	
	hgh=&rgb.b+((4+range*5)/2)%3;
	mid=&rgb.b+((1+range))%3;
	low=&rgb.b+((1+range*5)/2)%3;
	
	*hgh=(hsl.l+hsl.s)/2;
	*low=(hsl.l-hsl.s)/2;
	*mid=(hsl.h-range*255);
	
	if (range%2)
		*mid=255-*mid;
	
	*mid=(*mid)*hsl.s/255+(*low);
	
	return rgb;
}

inline int clrDif(RGBpix clr0, RGBpix clr1)
{
	clr0.r=abs(clr0.r-clr1.r);
	clr0.g=abs(clr0.g-clr1.g);
	clr0.b=abs(clr0.b-clr1.b);
	
	return (int)clr0.r*clr0.r+(int)clr0.g*clr0.g+(int)clr0.b*clr0.b;
}

inline void blend(RGBpix* clr1, RGBpix clr2, double blendFactor)
{ //blends the color it gets into the color it gets a pointer to
	
	if (blendFactor>=1)
		*clr1=clr2;
		
	else if (blendFactor<=0)
		return;
		
	else
	{
		clr1->r=clr1->r*(1-blendFactor)+clr2.r*blendFactor;
		clr1->g=clr1->g*(1-blendFactor)+clr2.g*blendFactor;
		clr1->b=clr1->b*(1-blendFactor)+clr2.b*blendFactor;
	}
}

inline void cmpst(RGBpix *clr1, RGBApix clr2)
{ //composts colors using alpha value
	if (clr2.a>=255)
	{
		clr1->r=clr2.r;
		clr1->g=clr2.g;
		clr1->b=clr2.b;
	}
	else if (clr2.a<=0)
		return;
	
	double blendFactor=clr2.a/255.0;
	
	clr1->r=clr1->r*(1-blendFactor)+clr2.r*blendFactor;
	clr1->g=clr1->g*(1-blendFactor)+clr2.g*blendFactor;
	clr1->b=clr1->b*(1-blendFactor)+clr2.b*blendFactor;
}


