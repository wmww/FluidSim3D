#include "FluidSim3D.h"
#include "../widap_base/WidapMath.h"

FluidSim3::Object::Object(FluidSim3 * simPtr, const char * objName, RGBpix objClr)
{
	sim=simPtr;
	nxtObj=sim->firstObj;
	prvObj=0;
	sim->firstObj=this;
	
	if (!objName)
		strcpy(name, "(no name)");
	else
		strcpy(name, objName);
	
	polyCount=0;
	directionalArea=0;
	dragCoefficient=0;
	firstWall=0;
	
	showWalls=1;
	showBB=0;
	showData=0;
	//showBBExpand=0.02;
	showBBExpand=0;
	showBBThick=0.15;
	showDataSize=1;
	showWallForce=0;
	wallClr=objClr;
	bbClr=objClr;
	dataClr=clr(0, 255, 0);
	
	framesSinceReset=0;
}

FluidSim3::Object::~Object()
{
	clearPointers();
	
	if (nxtObj)
		nxtObj->prvObj=prvObj;
	
	if (prvObj)
		prvObj->nxtObj=nxtObj;
	else
		sim->firstObj=nxtObj;
}

void FluidSim3::Object::addWall(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2)
{
	CollisionWall * ptr=firstWall;
	
	firstWall=new CollisionWall;
	++polyCount;
	
	firstWall->vert[0]=vert0;
	firstWall->vert[1]=vert1;
	firstWall->vert[2]=vert2;
	
	//a=b*h/2
	firstWall->area=dstToLine(firstWall->vert[2], firstWall->vert[0], firstWall->vert[1])*(firstWall->vert[1]-firstWall->vert[0]).dst()*0.5;
	
	firstWall->moving=0;
	
	//firstWall->dlta[0]=Vctr3<double>(-1, 0, 0);
	//firstWall->dlta[1]=Vctr3<double>(-2, 0, 0);
	//firstWall->dlta[2]=Vctr3<double>(-2, 0, 0);
	
	//firstWall->color=hsl2rgb(clrHSL(drand(0, 1530), 64, 255));
	//firstWall->color=clr(0, 0, 64);
	
	firstWall->ptrListStrt=0;
	
	firstWall->nxtWall=ptr;
}

void FluidSim3::Object::reset()
{
	CollisionWall * ptr=firstWall;
	int i;
	
	while (ptr)
	{
		for (i=0; i<3; ++i)
		{
			ptr->force[i]=Vctr3<double>();
			ptr->totalForce[i]=Vctr3<double>();
		}
		
		ptr=ptr->nxtWall;
	}
	
	framesSinceReset=0;
}

void FluidSim3::Object::advance()
{
	CollisionWall * wall=firstWall;
	
	totalDrag=Vctr3<double>();
	
	while (wall)
	{
		for (int i=0; i<3; ++i)
		{
			wall->totalForce[i]+=wall->force[i];
			wall->force[i]=Vctr3<double>();
			totalDrag+=wall->totalForce[i];
		}
		
		//movement needs to be switched to the whole object and various functions need to be called whenever anything moves
		//if (wall->moving)
		//	for (int i=0; i<3; ++i)
		//		wall->vert[i]+=wall->dlta[i];
			
		wall=wall->nxtWall;
	}
	
	totalDrag=totalDrag/(framesSinceReset+1);
	
	if (directionalArea)
	{
		dragCoefficient=directionalAreaVctr.normalized().dot(totalDrag)/(directionalArea*sim->inflowDensity*pow(directionalAreaVctr.normalized().dot(sim->startVel), 2));
	}
	
	framesSinceReset++;
}

void FluidSim3::Object::addToScene()
{
	CollisionWall * wall=firstWall;
	/*
	while (wall)
	{
		XYint verts[3];
		
		for (int i=0; i<3; ++i)
			verts[i]=mapOrthoPoint(wall->vert[i]);
		
		//img.triangle(verts[0], verts[1], verts[2], theme.perimClr, 0.5);
		
		//for (int i=0; i<3; ++i)
		//	img.line(verts[i], verts[(i+1)%3], theme.wireThick, theme.wireClr);
		
		//img.circle(mapOrthoPoint(wall->vert[0]), theme.wireThick*4, theme.wireClr, 0.5);
		//img.circle(mapOrthoPoint(wall->vert[1]), theme.wireThick*2, theme.wireClr, 0.5);
		
		wall=wall->nxt;
	}
	*/
	
	Vctr3<double> vertOffset=sim->dim;
	double buffer=(bbHgh-bbLow).dst()*showBBExpand;
	Vctr3<double> bufferVctr(buffer, buffer, buffer), low, hgh, textPos;
	vertOffset=vertOffset/2;
	low=bbLow-bufferVctr;
	hgh=bbHgh+bufferVctr;
	//hgh=sim->getDim();
	
	if (showWalls)
	{
		RGBpix color=wallClr;
		double force;
		//Vctr3<double> force;
		while (wall)
		{
			if (showWallForce)
			{
				force=clamp(grdnt(wall->totalForce[0].dst()+wall->totalForce[1].dst()+wall->totalForce[2].dst(), 0, sim->maxWallForce*wall->area*framesSinceReset, 0, 1), 0, 1);
				
				color=hsl2rgb(clrHSL((1-force)*255*4, 255, force*192+64));
				if (framesSinceReset<32)
					blend(&color, wallClr, (32.0-framesSinceReset)/9.0);
			}
			
			sim->scene3D.addTriangle(wall->vert[0]-vertOffset, wall->vert[1]-vertOffset, wall->vert[2]-vertOffset, color, false);
			
			wall=wall->nxtWall;
		}
	}
	
	if (showBB)
	{
		const int sections=4;
		const int divisions=1;
				
		sim->scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z)-vertOffset, Vctr3<double>(hgh.x, low.y, low.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z)-vertOffset, Vctr3<double>(low.x, hgh.y, low.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z)-vertOffset, Vctr3<double>(low.x, low.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		
		sim->scene3D.addPipe(Vctr3<double>(hgh.x, low.y, low.z)-vertOffset, Vctr3<double>(hgh.x, hgh.y, low.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(hgh.x, low.y, low.z)-vertOffset, Vctr3<double>(hgh.x, low.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, hgh.y, low.z)-vertOffset, Vctr3<double>(hgh.x, hgh.y, low.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, hgh.y, low.z)-vertOffset, Vctr3<double>(low.x, hgh.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, low.y, hgh.z)-vertOffset, Vctr3<double>(hgh.x, low.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, low.y, hgh.z)-vertOffset, Vctr3<double>(low.x, hgh.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		
		sim->scene3D.addPipe(Vctr3<double>(hgh.x, hgh.y, low.z)-vertOffset, Vctr3<double>(hgh.x, hgh.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(hgh.x, low.y, hgh.z)-vertOffset, Vctr3<double>(hgh.x, hgh.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
		sim->scene3D.addPipe(Vctr3<double>(low.x, hgh.y, hgh.z)-vertOffset, Vctr3<double>(hgh.x, hgh.y, hgh.z)-vertOffset, showBBThick, sections, 0, bbClr, true, divisions);
	}
	
	if (showData)
	{
		char text[512];
		//textPos=Vctr3<double>((hgh.x+low.x)/2.0, low.y-buffer, hgh.z)-vertOffset
		textPos=Vctr3<double>((hgh.x+low.x)/2.0, hgh.y, low.z-buffer)-vertOffset;
		sprintf(text, "\n%s\n %d triangles\n drag: %f\n area: %f\n DC: %f\n", name, polyCount, (double)totalDrag.dst(), directionalArea, dragCoefficient);
		sim->scene3D.addText(text, textPos, showDataSize, 1, dataClr, true);
	}
}

void FluidSim3::Object::clearPointers()
{
	CollisionWall * ptr=firstWall;
	WallPtr * wallPtr;
	
	while (ptr)
	{
		wallPtr=ptr->ptrListStrt;
		
		while (wallPtr)
		{
			ptr->ptrListStrt=wallPtr->nxtToSame;
			wallPtr->prvInVox->nxtInVox=wallPtr->nxtInVox; //this is safe because at the start of each vox there is a WallPtr that has no object so will never be hit here
			if (wallPtr->nxtInVox)
				wallPtr->nxtInVox->prvInVox=wallPtr->prvInVox;
			delete wallPtr;
			wallPtr=ptr->ptrListStrt;
		}
		
		ptr=ptr->nxtWall;
	}
}

void FluidSim3::Object::makePointers()
{
	CollisionWall * ptr=firstWall;
	Vctr3<int> lowLoc, hghLoc, loc;
	WallPtr * wallPtr, * wallPtrNxt;
	
	while (ptr)
	{
		lowLoc=(ptr->bbLow*sim->wallVoxDim/sim->dim).clamp(sim->wallVoxDim);
		hghLoc=(ptr->bbHgh*sim->wallVoxDim/sim->dim+Vctr3<int>(1, 1, 1)).clamp(sim->wallVoxDim);
		
		if (!ptr->moving)
		{
			for (loc.x=lowLoc.x; loc.x<hghLoc.x; ++loc.x)
			{
				for (loc.y=lowLoc.y; loc.y<hghLoc.y; ++loc.y)
				{
					for (loc.z=lowLoc.z; loc.z<hghLoc.z; ++loc.z)
					{
						wallPtr=sim->wallVoxMap+loc.x+loc.y*sim->wallVoxDim.x+loc.z*sim->wallVoxDim.y*sim->wallVoxDim.x;
						wallPtrNxt=wallPtr->nxtInVox;
						wallPtr->nxtInVox=new WallPtr;
						wallPtr->nxtInVox->prvInVox=wallPtr;
						wallPtr=wallPtr->nxtInVox;
						wallPtr->nxtInVox=wallPtrNxt;
						wallPtr->nxtToSame=ptr->ptrListStrt;
						ptr->ptrListStrt=wallPtr;
						wallPtr->wall=ptr;
					}
				}
			}
		}
		
		ptr=ptr->nxtWall;
	}
}

///TODO add moving wall bounding box

void FluidSim3::Object::setBoundingBox()
{
	CollisionWall * ptr=firstWall;
	
	if (!ptr)
	{
		bbLow=bbHgh=Vctr3<double>(0, 0, 0);
	}
	else
	{
		bbLow=bbHgh=ptr->vert[0]; //set them both to something inside or on the edge of the object
		
		while (ptr)
		{
			bbLow=Vctr3<double>::min(bbLow, ptr->bbLow=Vctr3<double>::min(Vctr3<double>::min(ptr->vert[0], ptr->vert[1]), ptr->vert[2]));
			bbHgh=Vctr3<double>::max(bbHgh, ptr->bbHgh=Vctr3<double>::max(Vctr3<double>::max(ptr->vert[0], ptr->vert[1]), ptr->vert[2]));
			
			ptr=ptr->nxtWall;
		}
	}
}

void FluidSim3::Object::calcDirectionalArea(int dim, Vctr3<double> direction)
{
	directionalAreaVctr=direction;
	
	Scene3D scene;
	WidapImage img(dim, dim);
	RGBpix color=clr(255, 255, 255);
	
	scene.setRenderImage(&img);
	
	CollisionWall * wall=firstWall;
	
	Vctr3<double> vertOffset=(bbLow+bbHgh)/2;
	
	while (wall)
	{
		scene.addTriangle(wall->vert[0]-vertOffset, wall->vert[1]-vertOffset, wall->vert[2]-vertOffset, color, true);
		
		wall=wall->nxtWall;
	}
	
	scene.cam->fov=(double)dim/(bbHgh-bbLow).dst();
	scene.cam->prsp=0;
	scene.cam->rot=Vctr3<double>(rad2deg(atan2(dst(direction.y, direction.x), direction.z)), 0, rad2deg(atan2(direction.x, direction.y)));
	
	img.clrFill(clr(0, 0, 0));
	
	scene.render();
	
	int i, onPix=0, pixNum=dim*dim;
	
	for (i=0; i<pixNum; i++)
	{
		if (img.bits[i].r)
			onPix++;
	}
	
	//directionalArea=(double)onPix*(bbHgh-bbLow).dstSquared()/(dim*dim);
	directionalArea=(double)onPix*(bbHgh-bbLow).dstSquared()/(dim*dim);
	
	img.save("a1");
}

