#include "../WidapCV.h"


ShapeFinder::Shape::Shape(int newMaxVertNum)
{
	vertNum=0;
	maxVertNum=newMaxVertNum;
	if (maxVertNum>0)
	{
		verts=new XYdbl[maxVertNum];
		vertLocs=new int[maxVertNum];
	}
}

ShapeFinder::Shape::~Shape()
{
	if (maxVertNum)
	{
		delete[] verts;
		delete[] vertLocs;
	}
}

void ShapeFinder::Shape::setMaxVertNum(int newMaxVertNum)
{
	vertNum=0;
	if (maxVertNum>0)
	{
		delete[] verts;
		delete[] vertLocs;
	}
	maxVertNum=newMaxVertNum;
	if (maxVertNum>0)
	{
		verts=new XYdbl[maxVertNum];
		vertLocs=new int[maxVertNum];
	}
}

double ShapeFinder::Shape::getDif(Shape* other)
{
	int offset, i;
	double dif, bestDif;
	
	if (other->vertNum!=vertNum)
	{
		std::cout << ">> tried to compare shapes with different numbers of verts <<\n";
		return 0;
	}
	
	if (!vertNum)
	{
		std::cout << ">> tried to compare shapes with no verts <<\n";
		return 0;
	}
	
	bestDif=10000;
	
	for (offset=0; offset<vertNum; ++offset)
	{
		dif=0;
		
		for (i=0; i<vertNum; ++i)
		{
			dif+=dst(verts[i], other->verts[(i+offset)%vertNum]);
		}
		
		if (dif<bestDif)
			bestDif=dif;
	}
	
	return bestDif;
}


ShapeFinder::ShapeFinder(WidapImage *newDbgImg)
{
	dbgImg=newDbgImg;
	checkPix=&ShapeFinder::dfltCheckPix;
	
	dwnSmpl=mkXY(DFLT_DWN_SMPL, DFLT_DWN_SMPL);
	ignoreMapLng=0;
	ignoreMapWdth=0;
	
	maxPerimLng=0; //this is important so setMaxPerimLng() works right
	setMaxPerimLng(DFLT_MAX_PERIM_LNG);
	
	shape.setMaxVertNum(DFLT_MAX_VERT_NUM);
	
	targetNum=0;
	targetThresh=10000; //any sufficiently large number
	
	pointinessBuffer=DFLT_BUFFER;
	minSgmntLng=DFLT_MIN_SGMNT;
	vertexThresh=DFLT_THRESH;
	
	freshImg=0;
	dim=mkXY(0, 0);
}

ShapeFinder::~ShapeFinder()
{
	setMaxPerimLng(0);
	
	if (ignoreMapLng)
		delete[] ignoreMap;
	
	if (targetNum)
		delete[] targets;
}

void ShapeFinder::nextImg(WidapImage *newImg)
{
	img=newImg;
	
	clearIgnoreMap(img->getDim(), dwnSmpl);
	
	srchLoc=mkXY(0, 0); //this is how things must start
	
	freshImg=1;
}

bool ShapeFinder::nextShape()
{
	bool foundShape=0;
	
	if (!freshImg && !ignoreMap[srchLoc.x/dwnSmpl.x+(srchLoc.y/dwnSmpl.y)*ignoreMapWdth])
	{
		std::cout << ">> failed to properly set the ignore map <<" << std::endl;
		bbLow=srchLoc;
		bbHgh=srchLoc;
		perimLng=0;
		shape.vertNum=0;
		return 0;
	}
	
	while ((ignoreMap[srchLoc.x/dwnSmpl.x+(srchLoc.y/dwnSmpl.y)*ignoreMapWdth] || !(foundShape=checkPix(img->pix(srchLoc)))) && (srchLoc.y=srchLoc.y+(!(srchLoc.x=(srchLoc.x+dwnSmpl.x)*(srchLoc.x+dwnSmpl.x<dim.x)))*dwnSmpl.y)<dim.y) {}
	
	freshImg=0;
	
	if (foundShape)
	{
		tracePerim();
		ignoreShape();
		calcPointiness();
		findVerts();
		return 1;
	}
	else
	{
		perimLng=0;
		shape.vertNum=0;
		return 0;
	}
}

void ShapeFinder::tracePerim()
{
	const int xDir[4]={-1, 0, 1,  0};
	const int yDir[4]={ 0, 1, 0, -1}; //used when tracing edge
	
	XYint loc=srchLoc;
	XYint locNew;
	int dir=0;
	int safty; //used to make sure loops terminate
	
	bbLow=loc;
	bbHgh=loc;
	
	perimLng=0;
	
	//make sure we have at least two pixels to work with
	if (!checkPix(img->pix(loc)) || !(
		(loc.x>0 		&& checkPix(img->pix(loc+mkXY(-1,  0)))) || 
		(loc.x<dim.x-1	&& checkPix(img->pix(loc+mkXY( 1,  0)))) ||
		(loc.y>0		&& checkPix(img->pix(loc+mkXY( 0, -1)))) ||
		(loc.y<dim.y-1	&& checkPix(img->pix(loc+mkXY( 0,  1))))))
	{
		perim[0]=loc;
		perimLng=1;
		return;
	}
		
	//go to a pixel on the edge
	while (loc.x>0 && checkPix(img->pix(loc+mkXY(-1, 0))))
		loc.x--;
	
	do
	{
		perim[perimLng]=loc;
		++perimLng;
		
		//it is necessary to calculate the perim bounding box to stop duplicate targets
		if (loc.x<bbLow.x) bbLow.x=loc.x;
		if (loc.x>bbHgh.x) bbHgh.x=loc.x;
		if (loc.y<bbLow.y) bbLow.y=loc.y;
		if (loc.y>bbHgh.y) bbHgh.y=loc.y;
		
		safty=0;
		
		do
		{
			if ((++dir)>=4)
				dir=0;
			
			locNew=mkXY(loc.x+xDir[dir], loc.y+yDir[dir]);
			
			if (++safty>4)
			{
				std::cout << ">> loop in tracePerim took to long (1 pixel sized vision target could be to blame) <<\n";
				perimLng=0;
				return;
			}
			
		} while (locNew.x<0 || locNew.x>=dim.x || locNew.y<0 || locNew.y>=dim.y || !checkPix(img->pix(locNew)));
		
		loc=locNew;
		
		if ((dir-=2)<0)
			dir+=4;
		
	} while ((loc!=perim[0]) && perimLng<maxPerimLng);
	
	if (perimLng>=maxPerimLng)
		std::cout << ">> perim overflowed <<" << std::cout;
	
	if (dbgImg)
		dbgImg->target(bbLow.x, bbHgh.x, bbLow.y, bbHgh.y, 12, clr(128, 0, 255), 0.5);
}

void ShapeFinder::ignoreShape()
{
	int x, y;
	XYint low, hgh;
	
	low.x=clamp(bbLow.x/dwnSmpl.x, 0, dim.x/dwnSmpl.x-1);
	low.y=clamp(bbLow.y/dwnSmpl.y, 0, dim.y/dwnSmpl.y-1);
	hgh.x=clamp(bbHgh.x/dwnSmpl.x, 0, dim.x/dwnSmpl.x-1);
	hgh.y=clamp(bbHgh.y/dwnSmpl.y, 0, dim.y/dwnSmpl.y-1);
	
	for (y=low.y; y<=hgh.y; ++y)
	{
		for (x=low.x; x<=hgh.x; ++x)
		{
			ignoreMap[x+y*ignoreMapWdth]=1;
		}
	}
}

void ShapeFinder::calcPointiness()
{
	int i;
	
	int loc1, loc2; //the two locations on the perimeter used to find the pointiness
	
	buffer=pointinessBuffer*perimLng; //set buffer relative to perimeter length
	
	if (buffer<MIN_BUFFER_PIX)
		buffer=MIN_BUFFER_PIX;
	
	if (buffer<1)
	{
		std::cout << ">> invalid buffer size of " << buffer << " <<" << std::endl;
	}
	else if (perimLng<1)
	{
		pointiness[0]=0;
	}
	else
	{
		for (i=0; i<perimLng; ++i) {
			
			loc1=i-buffer;
			while (loc1<0)
				loc1+=perimLng;
			
			loc2=i+buffer;
			while (loc2>=perimLng)
				loc2-=perimLng;
			
			pointiness[i]=dst(mkXYdbl(perim[i]), mkXYdbl(perim[loc1]), mkXYdbl(perim[loc2]));
		}
	}
	
	if (dbgImg)
	{
		int clrNum;
		
		for (i=0; i<perimLng; ++i)
		{
			clrNum=clamp(pointiness[i]*512/(buffer), 0, 512);
			
			*dbgImg->pix(perim[i])=hsl2rgb(clrHSL(512-clrNum, 255, 255));
		}
	}
}

void ShapeFinder::findVerts()
{
	//this function is much better commented then the others because it was lifted directly out of my FRC 2015 code
	
	bool	inc=1;			//if we are increasing
	int		perimPos;		//the current position on the perimeter
	int		vertListPos;	//our position in the vertex array (used for searching)
	int		lowestVert;		//which vertex has the lowest pointiness
	int		minLng=minSgmntLng*buffer; //the shortest distance there can be between two verts
	double	thresh=vertexThresh*buffer; //the least pointiness a vertex can be (will be raised if vert array fills up)
	XYint	vertLoc; //used when finding vert location
	
	shape.vertNum=0;
	
	for (perimPos=0; perimPos<perimLng-1; ++perimPos) //perimLng-1 because we compare each perim position to the one after it
	{
		if (pointiness[perimPos]<pointiness[perimPos+1])
		{
			inc=1; //we are increasing
		}
		else
		{
			if (inc && pointiness[perimPos]>=thresh) //if increasing is true then this is the first position with decreasing because inc would have been turned off otherwise
			{ //if it just changed from increasing to decreasing then we are at a relative max and should consider adding a vertex
				if (shape.vertNum && perimPos-shape.vertLocs[shape.vertNum-1]<minLng) //if we just had a vertex recently
				{
					if (pointiness[perimPos]>pointiness[shape.vertLocs[shape.vertNum-1]]) //if this new max is higher then the last one
					{
						shape.vertLocs[shape.vertNum-1]=perimPos; //replace the last vertex
					} //otherwise leave everything as is; no new vertex is added
				}
				else if (shape.vertNum && shape.vertLocs[0]+perimLng-perimPos<=minLng) //if we have gone all the way around and are close to the start
				{
					if (pointiness[shape.vertLocs[0]]<pointiness[perimPos]) //if our new vertex is better then the first one
					{
						for (vertListPos=0; vertListPos<shape.vertNum-1; ++vertListPos) //remove the lowest and shift all the rest backward to make room at the end
						{
							shape.vertLocs[vertListPos]=shape.vertLocs[vertListPos+1];
						}
						
						shape.vertLocs[shape.vertNum-1]=perimPos;
					} //otherwise leave everything as is; no new vertex is added
				}
				else //add vertex
				{
					if (shape.vertNum>=shape.getMaxVertNum()) //if the list is full then we must remove the least pointy vertex to make room
					{
						lowestVert=0;
						
						for (vertListPos=0; vertListPos<shape.vertNum; ++vertListPos) //search for the vert with the lowest pointiness
						{
							if (pointiness[shape.vertLocs[vertListPos]]<pointiness[shape.vertLocs[lowestVert]])
								lowestVert=vertListPos;
						}
						
						if ((thresh=pointiness[shape.vertLocs[lowestVert]])<pointiness[perimPos]) //if our new vertex is better then the worst old one
						{
							for (vertListPos=lowestVert; vertListPos<shape.vertNum-1; ++vertListPos) //remove the lowest and shift all the rest backward to make room at the end
							{
								shape.vertLocs[vertListPos]=shape.vertLocs[vertListPos+1];
							}
							
							shape.vertLocs[shape.vertNum-1]=perimPos;
						}
					}
					else //otherwise there is room in the list and we can just add a vertex
					{
						shape.vertLocs[shape.vertNum]=perimPos;
						++shape.vertNum;
					}
				}
			}
			inc=0;
		}
	}
	
	//now we have to run through the vertex list and set each vertexes XY cords
	
	for (vertListPos=0; vertListPos<shape.vertNum; ++vertListPos)
	{
		vertLoc=perim[shape.vertLocs[vertListPos]];
		shape.verts[vertListPos].x=(double)(vertLoc.x-bbLow.x)/(bbHgh.x-bbLow.x);
		shape.verts[vertListPos].y=(double)(vertLoc.y-bbLow.y)/(bbHgh.y-bbLow.y);
		
		if (dbgImg)
		{
			dbgImg->circle(vertLoc, 4.5, clr(0, 255, 128), 0.5);
			
			char txt[64];
			
			sprintf(txt, "%d", vertListPos);
			
			dbgImg->text(txt, vertLoc, 16, clr(0, 0, 255), 2);
		}
	}
}

int ShapeFinder::getClosestTarget()
{
	double dif, bestDif;
	int bestTrgt=0;
	int i;
	
	if (targetNum<=1)
	{
		std::cout << ">> tried to get closest target but the number of targets is " << targetNum << ". <<\n";
		return -1;
	}
	
	if (shape.vertNum!=shape.getMaxVertNum())
	{
		//std::cout << ">> vision target did not have enough verts for proper comparison <<\n";
		return -1;
	}
	
	bestDif=10000;
	
	for (i=0; i<targetNum; ++i)
	{
		dif=shape.getDif(&targets[i]);
		
		if (dif<bestDif)
		{
			bestDif=dif;
			bestTrgt=i;
		}
	}
	
	lastTargetDif=bestDif;
	
	if (bestDif<targetThresh)
		return bestTrgt;
	else
		return -1;
}

bool ShapeFinder::dfltCheckPix(RGBpix* pix)
{
	//return (pix->r+pix->g+pix->b>384);
	return (pix->r+pix->g+pix->b<50);
}

void ShapeFinder::clearIgnoreMap(XYint newDim, XYint newDwnSmpl)
{
	int i;
	int newIgnoreMapLng;
	
	newIgnoreMapLng=((newDim.x-1)/newDwnSmpl.x+1)*((newDim.y-1)/newDwnSmpl.y+1);
	ignoreMapWdth=(newDim.y-1)/newDwnSmpl.y+1;
	
	//if we need to delete and redeclare the ignore map
	if (newIgnoreMapLng!=ignoreMapLng)
	{
		if (ignoreMapLng)
			delete[] ignoreMap;
		
		dim=newDim;
		dwnSmpl=newDwnSmpl;
		ignoreMapLng=newIgnoreMapLng;
		
		if (ignoreMapLng)
			ignoreMap=new bool[ignoreMapLng];
	}
	
	//clear out the ignore map
	for (i=0; i<ignoreMapLng; ++i)
	{
		ignoreMap[i]=0;
	}
}

void ShapeFinder::setMaxPerimLng(int newMaxPerimLng)
{
	if (maxPerimLng)
	{
		delete[] perim;
		delete[] pointiness;
	}
	
	maxPerimLng=newMaxPerimLng;
	
	if (maxPerimLng)
	{
		perim=new XYint[maxPerimLng];
		pointiness=new float[maxPerimLng];
	}
	
	perimLng=0;
}

void ShapeFinder::setTrgtNum(int newTrgtNum)
{
	int vertNum;
	
	if (targetNum)
		delete[] targets;
	
	targetNum=newTrgtNum;
	
	targets=new Shape[targetNum];
	
	vertNum=shape.getMaxVertNum();
	
	for (int i=0; i<targetNum; ++i)
	{
		targets[i].setMaxVertNum(vertNum);
		targets[i].vertNum=vertNum;
	}
}


