#pragma once

#include "WidapImage.h"

class ImageStream
{
public:
	ImageStream(int maxDataLng);
	~ImageStream();
	
	void setFormat(char newFormat) {sendFormat=newFormat; reset(img);}
	
	double getProgress() {return progress;}
	
	void reset(WidapImage * newImg);
	int makePacket(char * newData); //returns length of packet, returns 0 when all done
	int processPacket(char * data); //returns how long the packet was
	
private:
	static const int DFLT_SND_FMT=2; //default format
	
	WidapImage * img;
	int sendPacketNum;
	double progress; //from 0 to 1 how far along we are sending
	
	char * data;
	int pos;
	
	int maxDataLng;
	char sendFormat;
	
	//format 1 is uncompressed
	void makeFormat1();
	void processFormat1();
	
	//format 2 has a bit of color down sampling and some other simple tricks
	void makeFormat2();
	void processFormat2();
	
	int twoByteInt();
	void twoByteInt(int inpt);
};

