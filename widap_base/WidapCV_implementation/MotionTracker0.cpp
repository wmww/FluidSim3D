#include "../WidapCV.h"

MotionTracker0::MotionTracker0(WidapImage *inptImg, int inptHScanLines, int inptVScanLines)
{
	int drctn, frame, line;
	
	slNum[0]=inptHScanLines;
	slNum[1]=inptVScanLines;
	
	img=inptImg;
	slLng[0]=img->getWdth();
	slLng[1]=img->getHght();
	
	for (drctn=0; drctn<2; ++drctn)
	{
		motionData[drctn]=new int[slLng[drctn]];
		motion[drctn]=0;
		
		for (frame=0; frame<2; ++frame)
		{
			slData[drctn][frame]=new int*[slNum[drctn]];
			
			for (line=0; line<slNum[drctn]; ++line)
			{
				slData[drctn][frame][line]=new int[slLng[drctn]];
			}
		}
	}
}

MotionTracker0::~MotionTracker0()
{
	int drctn, line, frame;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		delete[] motionData[drctn];
		
		for (frame=0; frame<2; ++frame)
		{
			for (line=0; line<slNum[drctn]; ++line)
			{
				delete[] slData[drctn][frame][line];
			}
		}
	}
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (frame=0; frame<2; ++frame)
		{
			delete[] slData[drctn][frame];
		}
	}
}

XYint MotionTracker0::getMotion()
{
	swapBuffers();
	
	loadData();
	
	//blurData(50);
	
	//smoothSpikes(1);
	
	//blurData(20);
	
	//smoothSpikes(1);
	
	//blurData(20);
	
	//takeDeriv();
	//takeDeriv();
	
	compareData();
	
	findMotion();
	
	return mkXY((int)motion[0], (int)motion[1]);
}

void MotionTracker0::swapBuffers()
{
	int drctn;
	int **ptr;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		ptr=slData[drctn][0];
		slData[drctn][0]=slData[drctn][1];
		slData[drctn][1]=ptr;
	}
}

void MotionTracker0::loadData()
{
	int drctn, line, pos;
	int level;
	int *x, *y;
	RGBpix pix0, pix1;
	
	if (img->getWdth()!=slLng[0] || img->getHght()!=slLng[1])
	{
		std::cout << ">> image being tracked changed size <<" << std::endl;
		return;
	}
	
	x=&pos; y=&level;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (line=0; line<slNum[drctn]; ++line)
		{
			level=(line+1)*slLng[!drctn]/(slNum[drctn]+1);
			pos=0;
			pix0=*img->pix(mkXY(*x, *y));
			
			for (pos=0; pos<slLng[drctn]; ++pos)
			{
				pix1=*img->pix(mkXY(*x, *y));
				slData[drctn][0][line][pos]=
					abs(pix1.r-pix0.r)+
					abs(pix1.b-pix0.b)+
					abs(pix1.g-pix0.g);
				pix0=pix1;
			}
		}
		
		x=&level; y=&pos;
	}
}

void MotionTracker0::smoothSpikes(int thresh)
{
	int drctn, line, pos;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (line=0; line<slNum[drctn]; ++line)
		{
			for (pos=1; pos<slLng[drctn]; ++pos)
			{
				if (slData[drctn][0][line][pos-1]-slData[drctn][0][line][pos]>thresh)// && abs(slData[drctn][0][line][pos-1]-slData[drctn][0][line][pos])>thresh)
					slData[drctn][0][line][pos]=slData[drctn][0][line][pos-1]-thresh;
				else if (slData[drctn][0][line][pos]-slData[drctn][0][line][pos-1]>thresh)// && abs(slData[drctn][0][line][pos-1]-slData[drctn][0][line][pos])>thresh)
					slData[drctn][0][line][pos]=slData[drctn][0][line][pos-1]+thresh;
				else 
					slData[drctn][0][line][pos]=slData[drctn][0][line][pos-1];
			}
		}
	}
}

void MotionTracker0::blurData(int pixNum)
{
	int drctn, line, pos;
	
	float blurSum;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (line=0; line<slNum[drctn]; ++line)
		{
			blurSum=0;
			
			for (pos=0; pos<pixNum; ++pos)
			{
				blurSum+=(float)slData[drctn][0][line][pos]/pixNum;
			}
			
			for (pos=0; pos<slLng[drctn]-pixNum; ++pos)
			{
				blurSum+=(float)slData[drctn][0][line][pos+pixNum]/pixNum;
				blurSum-=(float)slData[drctn][0][line][pos]/pixNum;
				slData[drctn][0][line][pos]=(int)blurSum;
			}
			
			for (pos=slLng[drctn]-pixNum; pos<slLng[drctn]; ++pos)
			{
				slData[drctn][0][line][pos]=(int)blurSum;
			}
		}
	}
}

void MotionTracker0::takeDeriv()
{
	int drctn, line, pos;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (line=0; line<slNum[drctn]; ++line)
		{
			for (pos=1; pos<slLng[drctn]; ++pos)
			{
				slData[drctn][0][line][pos-1]=slData[drctn][0][line][pos]-slData[drctn][0][line][pos-1];
			}
			
			slData[drctn][0][line][slLng[drctn]-1]=0;
		}
	}
}

void MotionTracker0::compareData()
{
	int drctn, line, pos;
	int score, bestPos[2];
	
	for (drctn=0; drctn<2; ++drctn)
	{
		for (line=0; line<slNum[drctn]; ++line)
		{
			score=0;
			bestPos[0]=0;
			bestPos[1]=0;
			
			for (pos=0; pos<slLng[drctn]; ++pos)
			{
				if (line==4 && pos)
					img->line(mkXY(pos, slData[drctn][0][line][pos]*2), mkXY(pos-1, slData[drctn][0][line][pos-1]*2), 1, clr(255, 0, 128));
					//img->line(mkLine(pos, slData[drctn][0][line][pos]*80+slLng[1]/2, pos-1, slData[drctn][0][line][pos-1]*80+slLng[1]/2), 1, clr(128, 0, 255));
				
				if (abs(slData[drctn][0][line][pos])>score)
				{
					score=abs(slData[drctn][0][line][pos]);
					bestPos[0]=pos;
				}
			}
			
			score=0;
			
			for (pos=0; pos<slLng[drctn]; ++pos)
			{
				if (abs(slData[drctn][1][line][pos])>score)
				{
					score=abs(slData[drctn][1][line][pos]);
					bestPos[1]=pos;
				}
			}
			
			motionData[drctn][line]=bestPos[0]-bestPos[1];
		}
	}
}

void MotionTracker0::findMotion()
{
	int drctn, chnl;
	long mtnSum;
	
	for (drctn=0; drctn<2; ++drctn)
	{
		mtnSum=0;
		
		for (chnl=0; chnl<3*slNum[drctn]; ++chnl)
		{
			mtnSum+=motionData[drctn][chnl];
		}
		
		if (slNum[drctn])
			motion[drctn]=mtnSum/(3*slNum[drctn]);
		else
			motion[drctn]=0;
	}
}


