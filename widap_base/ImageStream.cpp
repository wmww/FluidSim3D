
#include "ImageStream.h"
#include "WidapStd.h"

ImageStream::ImageStream(int newMaxDataLng)
{
	img=0;
	sendPacketNum=0;
	data=0;
	pos=0;
	
	maxDataLng=newMaxDataLng;
	
	sendFormat=DFLT_SND_FMT;
}

ImageStream::~ImageStream()
{
	
}

void ImageStream::reset(WidapImage * newImg)
{
	sendPacketNum=0;
	img=newImg;
}

int ImageStream::makePacket(char * newData)
{
	data=newData;
	pos=0;
	
	if (!data || !img || !img->getWdth() || !img->getHght() || !maxDataLng)
	{
		std::cout << ">> image stream did not have everything properly initialized <<\n";
	}
	
	data[pos]=sendFormat; ++pos;
	
	switch (sendFormat)
	{
	case 1:
		makeFormat1();
		break;
		
	case 2:
		makeFormat2();
		break;
		
	default:
		std::cout << ">> bad image stream format <<\n";
	}
	
	++sendPacketNum;
	
	return pos;
}

int ImageStream::processPacket(char * newData)
{
	data=newData;
	pos=0;
	
	if (!data || !img || !maxDataLng)
	{
		std::cout << ">> image stream did not have everything properly initialized <<\n";
	}
	
	++pos;
	switch (data[pos-1])
	{
	case 1:
		processFormat1();
		break;
		
	case 2:
		processFormat2();
		break;
		
	default:
		std::cout << ">> image stream got a packet with the unknown data format of " << (int)data[pos-1] << " <<\n";
	}
	
	return pos;
}


///format 1

void ImageStream::makeFormat1()
{
	int imgW, imgH;
	int headerLng=32;
	int sctns; //the number of sections to divide the image into (some number both horizontally and vertically so total number of packets is sctns^2)
	int x, y, w, h; //the location and size of the rectangle this packet contains
	int i, j, jFctr0, jFctr1;
	
	imgW=img->getWdth();
	imgH=img->getHght();
	
	//imgW*imgH/(sctns^2)=(maxLng-header)/3;
	sctns=ceil(sqrt(imgW*imgH*3.0/(maxDataLng-headerLng)));
	
	if (sendPacketNum>=sctns*sctns)
	{
		pos=0;
		return;
	}
	
	x=imgW*(sendPacketNum%sctns)/sctns;
	y=imgH*(sendPacketNum/sctns)/sctns;
	
	w=imgW*(sendPacketNum%sctns+1)/sctns-x;
	h=imgH/sctns;
	
	twoByteInt(imgW);
	twoByteInt(imgH);
	twoByteInt(x);
	twoByteInt(y);
	twoByteInt(w);
	twoByteInt(h);
	
	if (x+w>imgW || y+h>imgH)
		std::cout << ">> image stream made improper dimensions <<\n";
	
	RGBpix * pixDataStart=(RGBpix*)(data+pos);
	
	if (w*h*(int)sizeof(RGBpix)+pos>maxDataLng)
	{
		std::cout << ">> image stream packet will be too big, canceling... <<\n";
		return;
	}
	
	for (j=0; j<h; ++j)
	{
		jFctr0=j*w;
		
		jFctr1=(y+j)*imgW+x;
		
		for (i=0; i<w; ++i)
		{
			pixDataStart[jFctr0+i]=img->bits[jFctr1+i];
			//if (sendPacketNum%2)
			//	pixDataStart[jFctr0+i]=clr(255, 0, 0);
			//else
			//	pixDataStart[jFctr0+i]=clr(0, 255, 255);
		}
	}
	
	pos+=h*w*sizeof(RGBpix);
}

void ImageStream::processFormat1()
{
	int imgW, imgH;
	int x, y, w, h; //the location and size of the rectangle this packet contains
	int i, j, jFctr0, jFctr1;
	
	imgW=twoByteInt();
	imgH=twoByteInt();
	
	if (img->getWdth()!=imgW || img->getHght()!=imgH)
	{
		std::cout << "ImageStream resizing image to " << imgW << ", " << imgH << "to fit incoming image\n";
		img->newImage(mkXY(imgW, imgH));
	}
	
	x=twoByteInt();
	y=twoByteInt();
	w=twoByteInt();
	h=twoByteInt();
	
	if (x+w>imgW || y+h>imgH)
		std::cout << ">> image stream had improper dimensions in packet <<\n";
	
	RGBpix * pixDataStart=(RGBpix*)(data+pos);
	
	for (j=0; j<h; ++j)
	{
		jFctr0=j*w;
		
		jFctr1=(y+j)*imgW+x;
		
		for (i=0; i<w; ++i)
		{
			img->bits[jFctr1+i]=pixDataStart[jFctr0+i];
		}
	}
	
	pos+=h*w*sizeof(RGBpix);
	
	progress=grdnt(y+grdnt(x, 0, imgW-w, 0, h), 0, imgH, 0, 1);
}


///format 2

void ImageStream::makeFormat2()
{
	int imgW, imgH;
	int rawImgW, rawImgH;
	int headerLng=32;
	int sctns; //the number of sections to divide the image into (some number both horizontally and vertically so total number of packets is sctns^2)
	int x, y, w, h; //the location and size of the rectangle this packet contains
	int i, j, dataLineStrt, imgLineStrt;
	
	rawImgW=img->getWdth();
	rawImgH=img->getHght();
	
	imgW=rawImgW/2;
	imgH=rawImgH/2;
	
	//imgW*imgH/(sctns^2)=(maxLng-header)/3;
	sctns=ceil(sqrt(imgW*imgH*4.0/(maxDataLng-headerLng)));
	
	if (sendPacketNum>=sctns*sctns)
	{
		pos=0;
		return;
	}
	
	x=imgW*(sendPacketNum%sctns)/sctns;
	y=imgH*(sendPacketNum/sctns)/sctns;
	
	w=imgW*(sendPacketNum%sctns+1)/sctns-x;
	h=imgH/sctns;
	
	twoByteInt(imgW);
	twoByteInt(imgH);
	twoByteInt(x);
	twoByteInt(y);
	twoByteInt(w);
	twoByteInt(h);
	
	if (x+w>imgW || y+h>imgH)
		std::cout << ">> image stream made improper dimensions <<\n";
	
	if (w*h*4+pos>maxDataLng)
	{
		std::cout << ">> image stream packet will be too big, canceling... <<\n";
		std::cout << w*h*4+pos;
		return;
	}
	
	RGBpix pixBlock[4];
	unsigned char brightBlock[4];
	unsigned char * dataPixPtr;
	RGBpix avgClr;
	int k;
	
	for (j=0; j<h; ++j)
	{
		dataLineStrt=j*w*4;
		
		imgLineStrt=(y+j)*imgW*2*2+x*2;
		
		for (i=0; i<w; ++i)
		{
			dataPixPtr=(unsigned char *)(data+pos+dataLineStrt+i*4);
			
			pixBlock[0]=img->bits[imgLineStrt+i*2];
			pixBlock[1]=img->bits[imgLineStrt+i*2+1];
			pixBlock[2]=img->bits[imgLineStrt+i*2+rawImgW];
			pixBlock[3]=img->bits[imgLineStrt+i*2+rawImgW+1];
			
			for (k=0; k<4; ++k)
				brightBlock[k]=(pixBlock[k].r+pixBlock[k].g+pixBlock[k].b)/(3*4);
			
			//brightBlock[0]=0;
			//brightBlock[1]=(pixBlock[1].r+pixBlock[1].g+pixBlock[1].b)/12;
			//brightBlock[2]=0;
			//brightBlock[3]=0;
			
			avgClr.r=pixBlock[0].r/16;
			//avgClr.g=(pixBlock[0].g+pixBlock[1].g+pixBlock[2].g+pixBlock[3].g)/4;
			avgClr.b=pixBlock[0].b/16;
			
			dataPixPtr[0]=(brightBlock[0]<<2)|(brightBlock[1]>>4);
			dataPixPtr[1]=(brightBlock[1]<<4)|(brightBlock[2]>>2);
			dataPixPtr[2]=(brightBlock[2]<<6)|(brightBlock[3]>>0);
			
			dataPixPtr[3]=(avgClr.r<<4)|(avgClr.b);
		}
	}
	
	pos+=h*w*4;
}

void ImageStream::processFormat2()
{
	int imgW, imgH;
	int rawImgW, rawImgH;
	int x, y, w, h; //the location and size of the rectangle this packet contains
	int i, j, dataLineStrt, imgLineStrt;
	
	imgW=twoByteInt();
	imgH=twoByteInt();
	
	rawImgW=imgW*2;
	rawImgH=imgH*2;
	
	if (img->getWdth()!=rawImgW || img->getHght()!=rawImgH)
	{
		std::cout << "ImageStream resizing image to " << rawImgW << ", " << rawImgH << "to fit incoming image\n";
		img->newImage(mkXY(rawImgW, rawImgH));
	}
	
	x=twoByteInt();
	y=twoByteInt();
	w=twoByteInt();
	h=twoByteInt();
	
	if (x+w>imgW || y+h>imgH)
		std::cout << ">> image stream had improper dimensions in packet <<\n";
	
	RGBpix pixBlock[4];
	unsigned char brightBlock[4];
	unsigned char * dataPixPtr;
	RGBpix avgClr;
	int avgBright;
	int k;
	
	for (j=0; j<h; ++j)
	{
		dataLineStrt=j*w*4;
		
		imgLineStrt=(y+j)*imgW*2*2+x*2;
		
		for (i=0; i<w; ++i)
		{
			dataPixPtr=(unsigned char *)(data+pos+dataLineStrt+i*4);
			
			brightBlock[0]=dataPixPtr[0]>>2;
			brightBlock[1]=((dataPixPtr[0]<<4)|(dataPixPtr[1]>>4))&63;
			brightBlock[2]=((dataPixPtr[1]<<2)|(dataPixPtr[2]>>6))&63;
			brightBlock[3]=(dataPixPtr[2])&63;
			
			avgClr.r=(dataPixPtr[3]>>4)*16;
			avgClr.b=(dataPixPtr[3]&15)*16;
			
			avgClr.g=clamp(brightBlock[0]*4*3-avgClr.r-avgClr.b, 0, 255);
			
			avgBright=clamp(avgClr.r+avgClr.g+avgClr.b, 1, 255*3);
			
			for (k=0; k<4; ++k)
			{
				pixBlock[k].r=clamp(avgClr.r*brightBlock[k]*4*3/avgBright, 0, 255);
				pixBlock[k].g=clamp(avgClr.g*brightBlock[k]*4*3/avgBright, 0, 255);
				pixBlock[k].b=clamp(avgClr.b*brightBlock[k]*4*3/avgBright, 0, 255);
				
				//pixBlock[k].g=clamp(brightBlock[k]*4*3-pixBlock[k].r-pixBlock[k].b, 0, 255);
			}
			
			img->bits[imgLineStrt+i*2]=pixBlock[0];
			img->bits[imgLineStrt+i*2+1]=pixBlock[1];
			img->bits[imgLineStrt+i*2+rawImgW]=pixBlock[2];
			img->bits[imgLineStrt+i*2+rawImgW+1]=pixBlock[3];
		}
	}
	
	pos+=h*w*4;
	
	progress=grdnt(y+grdnt(x, 0, imgW-w, 0, h), 0, imgH, 0, 1);
}


///low level functions

int ImageStream::twoByteInt()
{
	int otpt=0;
	otpt+=((unsigned char)(data[pos]))*256;
	otpt+=((unsigned char)(data[pos+1]));
	pos+=2;
	return otpt;
}

void ImageStream::twoByteInt(int inpt)
{
	data[pos]=(unsigned char)(inpt/256);
	data[pos+1]=(unsigned char)(inpt%256);
	pos+=2;
}


