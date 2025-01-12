#pragma once

#include "WidapStd.h"
#include "WidapImage.h"
#include "WidapColors.h"

const int TRIG_CACHE_LNG=3600; //the resolution of the trig caches (arrays where sin and cos values are filled in at the beginning of the program for quick future reference)

extern bool trigCached; //used to determine if the trig caches have been made (should only be changed by cacheTrig())
extern double sinCache[TRIG_CACHE_LNG], cosCache[TRIG_CACHE_LNG];

void cacheTrig(); //this must be called at the beginning of the program or things that rely on the trig caches will break

inline double dst(XYdbl pt0, XYdbl a, XYdbl b);

class MotionTracker0 ///currently non functional
{ //uses scan lines to track motion
	//this is MotionTracker0 in case there are more motion tracking algorithms in the future
public:
	MotionTracker0(WidapImage *inptImg, int inptHScanLines, int inptVScanLines);
	~MotionTracker0();
	XYint getMotion();
	
private:
	WidapImage* img;
	int slLng[2]; //the length of the scan lines in both the horizontal and vertical directions, also the width and height of the input image
	int slNum[2]; //the number of horizontal and vertical scan lines
	double motion[2]; //motion in the two directions
	int **slData[2][2]; //all of the scan line data
		//1st dim: position 0 is horizontal, 1 is vertical
		//2nd dim: 0 is current, 1 is last time;
		//3rd dim (dynamic): points to an array [direction]ScanLines long of int pointers
		//4th dim (dynamic): each which point to a 1D array of ints which is the data for a single scan line
	int *motionData[2];
		//1st dim: position 0 is horizontal, 1 is vertical
		//2nd dim (dynamic): points to an array [direction]ScanLines*3 long of ints storing the motion of all the scan lines
	void swapBuffers(); //swap the prev and current frame data so the old current becomes prev and the data 2 cycles ago can overwritten as current
	void loadData(); //loads the data from the image
	void smoothSpikes(int thresh); //gets rid of the spikes and valleys
	void blurData(int pixNum); //does a simple blur on the data
	void takeDeriv(); //changes the data to its derivative; can be called multiple times;
	void compareData(); //compares the two frames of data to find motion for each field, saves result in motionData
	void findMotion(); //uses the data in motionData to find motion, stores its result in motion
};

class BoolImage
{ //an image made of bools

public:
	
	bool* bits; //a pointer to the start of the image
	
	int getWdth() {return wdth;} //returns the width
	int getHght() {return hght;} //returns the height
	
	//bool deleteBits; //set to one if this object created and thus needs to delete its bits
	
	BoolImage(); //constructor
	
	~BoolImage(); //destructor
	
	//the following functions make a new bool image from the rgb image they are sent a pointer to
	void hslRange(WidapImage *img, HSLpix low, HSLpix hgh); //if the color is between the two sent colors
	void rgbDlta(WidapImage *img, int thresh); //if the sum of the squares of the differences of the rgb color is greater then thresh
	
	bool* pix(unsigned int x, unsigned int y); //returns the contents of the specified pixel
	void newImage(int newWdth, int newHgth); //makes a new image of the specified size but does not fill it
	void trnsfr(WidapImage* othrImg, RGBpix clr0, RGBpix clr1); //transfers this image to an rgb image
	
private:
	
	int wdth, hght; //the dimensions of the bitmap
};

class KeyPoint ///currently semi functional
{ //a smart point that automatically finds a location with high contrast and helps to track it
public:
	struct ScanPix
	{
		RGBpix clr; //this pixels color
		XYint loc; //this pixels location
		double dlta; //the difference between this pix and the ones on either side of it
	};
	
	KeyPoint(); //constructor
	~KeyPoint(); //destructor
	
	//finds a position with high contrast
	void findPos(WidapImage *img, XYdbl strtPos, double strtRds, double endRds, int steps, int newScanRes);
	
	//finds the position that best matches the old location
	//brute force
	void updtPosBF(WidapImage *img, XYdbl motionGuess, int srchSize, int increment);
	//recursion (starts big and zooms in) (does not work well)
	void updtPosR(WidapImage *img, XYdbl motionGuess, double strtRds, int steps);
	//multiple levels of resolution (similar to brute force, but optimized)
	void updtPosML(WidapImage *img, XYdbl motionGuess, int lvl1SrchSize, int lvl2SrchSize, double sampleOverlap);
	
	//accessors
	XYdbl getPos() {return pos;}
	double getX() {return pos.x;}
	double getY() {return pos.y;}
	double getRds() {return rds;}
	int getScanRes() {return scanRes;}
	ScanPix* getScanCrcl() {return scanCrcl;}
	double getError() {return error;}
	bool getIfCloseToEdge() {return pos.x-rds-1<0 || pos.x+rds+2>wdth || pos.y-rds-1<0 || pos.y+rds+2>hght;}
	
	static WidapImage *dbgImg; //an image to draw debugging graphics to
	
private:
	void advanceData(); //move this frame's data to last frame and allocate and delete memory as needed
	void calcCrclData(WidapImage *img); //uses the classes internal vars to get the data (all except dlta) for a single location and size
	void calcDltaWithSelf(); //calculates delta values based on similarity with nearby pixels
	double calcDltaWithLast(); //calculates delta values based on similarity with the last frame
	void refinePos(WidapImage *img, int srchSize, int increment); //searches the area and moves pos to the best position
	void movePos(); //uses the circle data that has presumably already been gotten to set a new and improved position of high contrast
	
	void rndrToImg(); //show this key point on the debug image
	
	int wdth, hght; //the dimensions of the image
	XYdbl pos; //the position of the key point
	double rds; //the radius of the scan circle
	int scanRes; //how many pixels there are in the scan circle
	ScanPix *scanCrcl; //the colors of the pixels in the scan circle
	ScanPix *lastScanCrcl; //the scan data from the previous frame
	double error;
};

class ShapeFinder
{ //you give it an image and it returns the vertices of all the shapes
	
public:
	
	class Shape
	{
	public:
		Shape(int newMaxVertNum=0);
		~Shape();
		void setMaxVertNum(int newMaxVertNum);
		double getDif(Shape* other); //returns the difference between this shape and another
		
		int getMaxVertNum() {return maxVertNum;}
		
		int vertNum; //the number of verts that are actually being used
		XYdbl *verts; //points to an array of verts maxVertNum long but the number of significant verts is vertNum, the rest are undefined and could be anything
		int *vertLocs; //the location on the perim of each vertex
	private:
		int maxVertNum; //the length of the vertex array
	};
	
	Shape shape;
	XYint bbLow, bbHgh; //the perimeter bounding box
	
	int targetNum;
	Shape *targets; //ideal vision targets
	double lastTargetDif; //the difference between the shape and the closest vision target
	double targetThresh; //the threshold over which getClosestTarget() returns -1
	
	double pointinessBuffer; //how many pixels the samples used to find pointiness are apart (proportional to perimLng)
	double minSgmntLng; //the shortest distance between two verts (proportional to buffer)
	double vertexThresh; //the lowest pointiness a vertex can be (proportional to buffer)
	
	double buffer; //the actual buffer used when going around the perimeter
		//set in calcPointiness to pointinessBuffer*perimLng but can not be less then MIN_BUFFER_PIX
	
	ShapeFinder(WidapImage *newDbgImg=0);
	~ShapeFinder();
	
	void setDbgImg(WidapImage *newDbgImg=0) {dbgImg=newDbgImg;}
	
	void nextImg(WidapImage *newImg); //it is important to call this every time the image changes, even if the old pointer is still valid
	bool nextShape(); //advances to the next shape in the image
		//must be called once after nextImg to get first shape
		//if there is another shape, and it finds it, returns 1
		//if no more shapes are found and thus vertNum is 0 returns 0
	
	void tracePerim(); //traces the perimeter starting at loc
	void ignoreShape(); //fill in the ignore map with the current perimeter bounding box
	void calcPointiness(); //find the pointiness of everywhere on the perimeter
	void findVerts(); //find all the verts with pointiness above the thresh, if too many verts, find the maxVertNum best of them
	
	int getClosestTarget(); //returns the number of the vision target that is closest to the on which has presumably bid found
	
	static bool dfltCheckPix(RGBpix *pix); //returns if the given pixel is on the location
	
	void setCheckPixFnctn(bool (*newCheckPix)(RGBpix*)) {checkPix=newCheckPix;}; //set the function for checking if color is on shape
	void clearIgnoreMap(XYint newDim, XYint newDwnSmpl); //sets ignore map to all 0s, also how you change the down sampling
		//if dimensions or down sampling do not match current, first deletes and reallocates ignore map 
	void setMaxPerimLng(int newMaxPerimLng); //sets the maximum perimeter length and manages related memory
	void setTrgtNum(int newTrgtNum); //sets the number of vision targets and manages memory
	
	XYint getDim() {return dim;}
	int getMaxPerimLng() {return maxPerimLng;}
	int getPerimLng() {return perimLng;}
	XYint* getPerim() {return perim;}
	float* getPointiness() {return pointiness;}
	
private:
	static const int DFLT_DWN_SMPL=8; //dwnSmple in both directions if you don't set it	
	static const int DFLT_MAX_PERIM_LNG=4000; //maxPerimLng if you don't set it	
	static const int DFLT_MAX_VERT_NUM=12; //maxVertNum if you don't set it
	static constexpr double DFLT_BUFFER=0.05; //pointinessBuffer if you don't set it
	static constexpr double DFLT_MIN_SGMNT=1; //minSgmntLng if you don't set it
	static constexpr double DFLT_THRESH=0; //vertexThresh if you don't set it
	static const int MIN_BUFFER_PIX=4; //the minimum number of pixels the buffer can be (must be greater then zero)
	
	XYint dim; //dimensions of the image
	WidapImage *dbgImg; //image to draw debugging data to
	WidapImage *img; //the image on which to find the shapes
	XYint srchLoc; //the location at which we are searching
	int ignoreMapLng, ignoreMapWdth; //the total length of the ignore map and its width (width is important for quick location access))
	bool *ignoreMap; //set to point at an array of bools used to ignore places objects have already been found
		//the ignore map is down sampled 4 times in both dimensions so it is 16 times smaller and thus quicker
	
	bool (*checkPix)(RGBpix*);
	bool freshImg; //if no shapes have been found in the current image yet, used for error catching
	
	XYint dwnSmpl; //how much down sampling to have in both directions
	int maxPerimLng, perimLng; //length of the perimeter array and how much of it is actually being used respectively
	XYint *perim; //the XY cords for everywhere on the perimeter of an object
	float *pointiness; //same lengths as the perimeter, the pointiness is used to find verts
};

inline double dst(XYdbl pt0, XYdbl a, XYdbl b) //shortest distance between a line and a point
{
	XYdbl pt1;
	double m;
	
	if (a.x==b.x) { //slope would be undefined
		
		pt1.x=a.x;
		pt1.y=pt0.y;
	}
	else {
		
		m=(b.y-a.y)/(b.x-a.x); //find the slope of the line connecting point 1 and 2
		
		pt1.x=(m*(pt0.y-a.y+m*a.x)+pt0.x)/(m*m+1);
		pt1.y=m*(pt1.x-a.x)+a.y; //find the point shared by the line connecting 1 and 2 and the line perpendicular to that which runs through 3
	}
	
	return dst(pt1, pt0); //find the distance between point 3 and the line between 1 and 2
}

