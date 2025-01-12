#include "../WidapCV.h"

WidapImage *KeyPoint::dbgImg=0;

KeyPoint::KeyPoint()
{
	if (!trigCached)
		cacheTrig();
	
	pos=mkXY(0.0, 0.0);
	rds=0;
	scanRes=0;
	scanCrcl=0;
	lastScanCrcl=0;
	wdth=0;
	hght=0;
	//dbgImg=0;
}

KeyPoint::~KeyPoint()
{
	if (scanCrcl)
		delete[] scanCrcl;
	
	if (lastScanCrcl)
		delete[] lastScanCrcl;
}

void KeyPoint::findPos(WidapImage *img, XYdbl strtPos, double strtRds, double endRds, int steps, int newScanRes)
{
	int i;
	XYdbl lastPos;
	
	scanRes=newScanRes;
	if (lastScanCrcl)
		delete[] lastScanCrcl;
	lastScanCrcl=0;
	if (scanCrcl)
		delete[] scanCrcl;
	scanCrcl=new ScanPix[scanRes];
	pos=strtPos;
	
	for (i=0; i<steps; ++i)
	{
		//dspImg.circle(pos, rds, clr(255, 0, 128), 0.05);
		rds=grdnt(i, 0, steps, strtRds, endRds);
		calcCrclData(img);
		calcDltaWithSelf();
		lastPos=pos;
		movePos();
		if (dbgImg)
			dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
	}
	
	rds=endRds;
	calcCrclData(img);
	
	rndrToImg();
}

void KeyPoint::updtPosBF(WidapImage *img, XYdbl motionGuess, int srchSize, int increment)
{
	XYdbl lastPos;
	
	advanceData();
	
	pos.x+=motionGuess.x;
	pos.y+=motionGuess.x;
	
	lastPos=pos;
	
	refinePos(img, srchSize, increment);
	
	if (dbgImg)
		dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
	
	rndrToImg();
}

void KeyPoint::updtPosR(WidapImage *img, XYdbl motionGuess, double strtRds, int steps)
{
	int i;
	XYdbl lastPos;
	double left, rght, btm, top;
	double netDlta0, netDlta1;
	double endRds=rds;
	
		//does not work if 2, makes two circles with no overlap at 4 in between, scan circles have overlap
	const double rdsFctr=2.25;
	
	advanceData();
	
	pos.x+=motionGuess.x;
	pos.y+=motionGuess.x;
	left=pos.x-strtRds;
	rght=pos.x+strtRds;
	btm=pos.y-strtRds;
	top=pos.y+strtRds;
	
	for (i=0; i<steps; ++i)
	{
		lastPos=pos;
		
		rds=(rght-left)/rdsFctr;
		
		pos.x=left+rds;
		calcCrclData(img);
		netDlta0=calcDltaWithLast();
		
		pos.x=rght-rds;
		calcCrclData(img);
		netDlta1=calcDltaWithLast();
		
		if (netDlta0>netDlta1)
			left=rght-rds*2;
		else
			rght=left+rds*2;
		
		pos.x=left+(rght-left)/2.0;
		
		if (dbgImg)
			dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
		lastPos=pos;
		
		pos.y=btm+rds;
		calcCrclData(img);
		netDlta0=calcDltaWithLast();
		
		pos.y=top-rds;
		calcCrclData(img);
		netDlta1=calcDltaWithLast();
		
		if (netDlta0>netDlta1)
			btm=top-rds*2;
		else
			top=btm+rds*2;
		
		pos.y=btm+(top-btm)/2.0;
		
		if (dbgImg)
			dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
	}
	
	rds=endRds;
	calcCrclData(img);
	
	rndrToImg();
}

void KeyPoint::updtPosML(WidapImage *img, XYdbl motionGuess, int lvl1SrchSize, int lvl2SrchSize, double sampleOverlap)
{
	XYdbl lastPos;
	double increment=(1-sampleOverlap)*rds*2;
	
	advanceData();
	
	pos.x+=motionGuess.x;
	pos.y+=motionGuess.x;
	
	lastPos=pos;
	
	refinePos(img, lvl1SrchSize, increment);
	
	if (dbgImg)
		dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
	
	lastPos=pos;
	
	refinePos(img, lvl2SrchSize, 1);
	
	if (dbgImg)
		dbgImg->line(mkXYint(lastPos), mkXYint(pos), 2, clr(0, 128, 255));
	
	rndrToImg();
}

void KeyPoint::advanceData()
{
	if (!scanCrcl)
	{
		std::cout << ">> called advanceData() but key point does not yet have any data, call findPos() first <<" << std::endl;
		return;
	}
	
	///keep first set of data
	/*if (!lastScanCrcl)
		lastScanCrcl=scanCrcl;
	else
		delete[] scanCrcl;
	scanCrcl=new ScanPix[scanRes]; //scanCrcl is never deleted, its just passed to lastScanCrcl which is later deleted
	*/
	
	///each frame, look for last frame's location
	if (lastScanCrcl)
		delete[] lastScanCrcl;
	lastScanCrcl=scanCrcl;
	scanCrcl=new ScanPix[scanRes]; //scanCrcl is never deleted, its just passed to lastScanCrcl which is later deleted
}

void KeyPoint::calcCrclData(WidapImage *img)
{
	int i;
	int cachePos;
	
	wdth=img->getWdth();
	hght=img->getHght();
	
	for (i=0; i<scanRes; ++i)
	{
		cachePos=i*TRIG_CACHE_LNG/scanRes;
		scanCrcl[i].loc.x=clamp(cosCache[cachePos]*rds+pos.x, 1, wdth-2);
		scanCrcl[i].loc.y=clamp(sinCache[cachePos]*rds+pos.y, 1, hght-2);
		scanCrcl[i].clr=*(img->bits+scanCrcl[i].loc.x+scanCrcl[i].loc.y*wdth);
	}
}

void KeyPoint::calcDltaWithSelf()
{
	int i;
	
	for (i=0; i<scanRes; ++i)
	{
		//std::cout << clrDif(scanCrcl[i].clr, scanCrcl[(i+1)%scanRes].clr) << std::endl;
		
		//scanCrcl[i].dlta=(clrDif(scanCrcl[i].clr, scanCrcl[(i+1)%scanRes].clr)>1000.0);
		
		scanCrcl[i].dlta=clrDif(scanCrcl[i].clr, scanCrcl[(i+1)%scanRes].clr)/768.0;
		//scanCrcl[i].dlta=scanCrcl[i].dlta*scanCrcl[i].dlta;
	}
}

double KeyPoint::calcDltaWithLast()
{
	int i;
	double netDlta=0;
	
	if (!lastScanCrcl)
	{
		std::cout << ">> tried to calc delta with last but there is no last data <<\n";
		return 0.0;
	}
	
	for (i=0; i<scanRes; ++i)
	{
		scanCrcl[i].dlta=clrDif(scanCrcl[i].clr, lastScanCrcl[i].clr)/768.0;
		netDlta+=(scanCrcl[i].dlta=scanCrcl[i].dlta*scanCrcl[i].dlta);
	}
	
	return netDlta;
}

void KeyPoint::refinePos(WidapImage *img, int srchSize, int increment)
{
	double left, rght, btm, top;
	long score, bestScore;
	XYdbl bestPos;
	
	left=pos.x-srchSize;
	rght=pos.x+srchSize+0.5;
	btm=pos.y-srchSize;
	top=pos.y+srchSize+0.5;
	bestScore=5792082; //any reasonably large number
	bestPos=pos;
	
	for (pos.y=btm; pos.y<top; pos.y+=increment)
	{
		for (pos.x=left; pos.x<rght; pos.x+=increment)
		{
			calcCrclData(img);
			score=calcDltaWithLast();
			
			if (score<bestScore)
			{
				bestScore=score;
				bestPos=pos;
			}
		}
	}
	
	error=bestScore;
	pos=bestPos;
	calcCrclData(img);
}

void KeyPoint::movePos()
{
	int i;
	
	XYdbl newPos=mkXY(0.0, 0.0);
	double ttlWght=0; //the total weight for the weighted average
	
	for (i=0; i<scanRes; ++i)
	{
		//scanCrcl[i].dlta=1;
		newPos.x+=scanCrcl[i].loc.x*scanCrcl[i].dlta;
		newPos.y+=scanCrcl[i].loc.y*scanCrcl[i].dlta;
		ttlWght+=scanCrcl[i].dlta;
	}
	
	//std::cout << ttlWght << std::endl;
	
	error=ttlWght;
	
	if (ttlWght!=0)
	{
		pos.x=newPos.x/ttlWght+0.5;
		pos.y=newPos.y/ttlWght+0.5; //+0.5 should not be necessary since everything is a double but it makes stuff work better
	}
	else
	{
		std::cout << ">> key point had a scan circle all of exactly the same color <<\n";
	}
}

void KeyPoint::rndrToImg()
{
	int i;
	RGBpix crclClr;
	
	if (!dbgImg)
		return;
	
	if (lastScanCrcl==0)
		crclClr=clr(255, 0, 0);
	else
		crclClr=clr(0, 255, 128);
	
	dbgImg->circle(mkXYint(pos), rds, crclClr, 0.4);
	
	for (i=0; i<scanRes; ++i)
	{
		*dbgImg->pix(scanCrcl[i].loc)=clr(255, 255, 255);
	}
}


