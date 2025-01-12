#include "FluidSim3D.h"
#include <thread> //used for calling threads
#include <sys/sysinfo.h> //for finding free system memory to make sure I have enough
#include "../widap_base/WidapMath.h"
#include <fstream> //for loading stl files

FluidSim3::FluidSim3()
{
	NULL_VOXEL.density=0;
	NULL_VOXEL.cntr=Vctr3<float>(0, 0, 0);
	NULL_VOXEL.vel=Vctr3<float>(0, 0, 0);
	frame=0;
	voxNum=0;
	voxMap=0;
	incoming=0;
	voxMapAvg=0;
	areaScale=1;
	boundaryObj=0;
	firstObj=0;
	wallVoxDimScale=1;
	wallVoxMap=0;
	
	scene3D.setRenderImage(&img);
	transScene.setRenderImage(new WidapImage(img.getWdth(), img.getHght()));
	//zBuffer=0;
	
	printUpdates=1;
	
	/*camRotX=0;
	camRotZ=0;
	camZoom=1;
	scale=1;
	camFov=60;
	camPos=Vctr3<double>(0, 0, 0);*/
	
	cam=&scene3D.cam0;
	transScene.cam=cam;
	
	renderBoundry=0;
	renderWalls=1;
	renderFluid=0;
	renderFlowLines=0;
	calcObjAreaRes=0;
	maxWallForce=1.0;
	maxDisplayDensity=2.0;
	
	startTotalMass=0;
	totalMass=0;
	addedMass=0;
	escapedMass=0;
	triCount=0;
	
	theme.bkndClr=clr(255, 255, 255);
	theme.fluidClr=clr(64, 64, 64);
	theme.wireClr=clr(0, 0, 0);
	theme.wireThick=4;
	theme.perimClr=clr(192, 192, 192);
	theme.perimThick=6;
	flowLinesClr=clr(192, 192, 192);
	
	///Fluid flow settings
	
	startDensity=1; //density to start every voxel at
	inflowDensity=startDensity; //density to start every voxel at
	startVel=Vctr3<float>(); //initial velocity
	
	voxDivisPerDensity=1; //how many particles to divide each density unit into
	voxDivisMax=4; //max number of particles to divide the box into
	
	randomMove=0; //randomness to add to the velocity
	pressureForce=0; //force of pressure
	wallBounciness=0.5; //how bouncy walls are
}

FluidSim3::~FluidSim3()
{
	while (firstObj)
	{
		delete firstObj; //the maintaining of the list is handled in Object destructor
	}
	
	uninit();
}

void FluidSim3::setDim(Vctr3<int> newDim, double newAreaScale)
{
	if (voxNum)
		uninit();
	
	areaScale=newAreaScale;
	
	dim=newDim*newAreaScale;
}

void FluidSim3::loadStl(const char * filename, Vctr3<double> center, double scale, RGBpix color)
{
	std::ifstream fileStream; //the stream to read the file
	unsigned char buffer[80], *data; //a buffer where the data is stored
	unsigned int triNum; //the total number of triangles in the file
	int i, j, k;
	Vctr3<double> verts[3];
	double val;
	unsigned long base; //don't think a long is necessary but best be on the safe side
	unsigned char power;
	
	std::cout << "loading '" << filename << "'...\n";
	
	scale*=areaScale;
	
	fileStream.open(filename, std::ifstream::binary);
	
	if (!fileStream.is_open()) //file did not load
	{
		std::cout << "FluidSim3::loadStl(): failed to load stl '" << filename << "'\n";
		return;
	}
	
	fileStream.read((char *)buffer, 80); //if the first 5 bytes spell out 'solid' this is an ASCII stl which I do not currently support
	
	buffer[5]=0;
	
	if (!strcmp((char *)buffer, "solid")) //this is an ASCII stl
	{
		std::cout << "FluidSim3::loadStl(): '" << filename << "' is an ASCII stl, which is not supported\n";
		fileStream.close();
		return;
	}
	
	fileStream.read((char *)buffer, 4); //a 4 bit integer here is the total number of triangles
	
	triNum=buffer[3]*256*256*256+buffer[2]*256*256+buffer[1]*256+buffer[0];
	
	std::cout << "there are " << triNum << " triangles in the file\n";
	
	if (triNum>1048576)
	{
		std::cout << "there appear to be too many triangles, aborting\n";
		return;
	}
	
	new Object(this, filename, color);
	
	//strcpy(firstObj->name, filename);
	
	//do the actual loading of the triangles
	for (i=0; i<(int)triNum; i++)
	{
		//according to Wikipedia, this is the format:
		//REAL32[3] - Normal vector (I ignore this
		//REAL32[3] - Vertex 1 (these are floating point)
		//REAL32[3] - Vertex 2 (these are floating point)
		//REAL32[3] - Vertex 3 (these are floating point)
		//UINT16 - Attribute byte count (should be 0)
		
		//10 2B 1C C0 = -2.44013
		
		//00010000 00101011 00011100 11000000 = -2.44013
		//00000111 11000000 00000000 00000000 = 0
		//10011001 01100011 11100000 01000000 = 7.01216
		
		fileStream.read((char *)buffer, 50); //this is all the data for a triangle
		
		for (j=0; j<3; j++)
		{
			for (k=0; k<3; k++)
			{
				data=buffer+12+j*12+k*4;
				
				base=(((data[2]<<1)>>1) | (1<<7))*256*256+data[1]*256+data[0];
				
				power=((data[3]<<1) | (data[2]>>7));
				
				val=(base/(256*256*128.0))*pow(2, power-127);
				
				if (data[3]>>7) //sign
					val*=-1;
				
				*(&(verts[j].x)+k)=val;
			}
		}
		
		firstObj->addWall(verts[0]*scale+center, verts[1]*scale+center, verts[2]*scale+center);
	}
	
	fileStream.close();
	
	std::cout << "\n";
}

void FluidSim3::addBoundaryObj(bool xNeg, bool xPos, bool yNeg, bool yPos, bool zNeg, bool zPos)
{
	if (boundaryObj)
		delete boundaryObj; //boundaryObj is in the linked list but the destructor will take care of safely removing it. yay OOP.
	
	boundaryObj=new Object(this, "Boundary", clr(128, 128, 128));
	
	boundaryObj->showWalls=0;
	boundaryObj->showBB=0;
	boundaryObj->showData=0;
	
	Vctr3<double> hgh(dim.x-1, dim.y-1, dim.z-1);
	
	if (xNeg)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(0, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(0, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
	}
	
	if (yNeg)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(0, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(1, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5));
	}
	
	if (zNeg)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(0, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(1, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5));
	}
	
	if (xPos)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(1, 0, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(1, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
	}
	
	if (yPos)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(0, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(1, 1, 0)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
	}
	
	if (zPos)
	{
		boundaryObj->addWall(hgh*Vctr3<double>(0, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
		boundaryObj->addWall(hgh*Vctr3<double>(1, 0, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(0, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5), hgh*Vctr3<double>(1, 1, 1)+Vctr3<double>(0.5, 0.5, 0.5));
	}
}

void FluidSim3::init(bool ignoreLowRam)
{
	int i;
	
	if (!dim.volume())
	{
		std::cerr << "could not init; bad dimensions\n";
		return;
	}
	
	if (printUpdates)
	{
		std::cout << "initializing " << PROG_NAME << "...\n"
				  "dimensions: " << dim.x << " by " << dim.y << " by " << dim.z << "\n";
	}
	
	///find out if we have enough free RAM
	unsigned long requiredRam, totalRam;
	struct sysinfo info;
	sysinfo(&info);
	totalRam=(unsigned long)(info.totalram*info.mem_unit);
	//freeRam=(unsigned long)((info.freeram+info.bufferram)*info.mem_unit); //this gives a much lower value then it should, probably due to the RAM cache
	requiredRam=(unsigned long)(dim.volume()*2*sizeof(FluidVoxel));
	
	if (printUpdates)
	{
		std::cout << "total RAM: " << totalRam/1024/1024.0/1024 << " GB\n";
		//std::cout << "free RAM: " << freeRam/1024/1024/1024 << "GB\n";
		std::cout << "required RAM: " << requiredRam/1024/1024.0/1024 << " GB (" << int((requiredRam/1024/1024.0)*100/(totalRam/1024/1024.0)) << "%)\n";
	}
	
	if (requiredRam>totalRam*0.6)
	{
		std::cerr << "FluidSim3::init(): simulation requires too much RAM!\n";
		
		if (ignoreLowRam)
		{
			std::cerr << "FluidSim3::init(): ignoreLowRam is true, so continuing anyway...\n";
		}
		else
		{
			std::cerr << "FluidSim3::init(): aborting init\n";
			
			return;
		}
		
	}
	
	if (printUpdates)
		std::cout << "RAM check passed, allocating dynamic memory...\n";
		
	frame=0;
	voxNum=dim.volume();
	wallVoxDim=dim/wallVoxDimScale;
	
	voxMap=new FluidVoxel[voxNum];
	
	if (!voxMap) //dynamic memory error
	{
		std::cerr << "FluidSim3::init(): voxel voxMap failed to initialize\n";
		uninit();
		return;
	}
	
	incoming=new FluidVoxel[voxNum];
	
	if (!incoming) //dynamic memory error
	{
		std::cerr << "FluidSim3::init(): voxel incoming failed to initialize\n";
		uninit();
		return;
	}
	
	voxMapAvg=new FluidVoxel[voxNum];
	
	if (!voxMapAvg) //dynamic memory error
	{
		std::cerr << "FluidSim3::init(): voxel voxMapAvg failed to initialize\n";
		uninit();
		return;
	}
	
	wallVoxMap=new Object::WallPtr[wallVoxDim.volume()];
	
	if (!wallVoxMap) //dynamic memory error
	{
		std::cerr << "FluidSim3::init(): wallVoxMap failed to initialize\n";
		uninit();
		return;
	}
	
	for (i=0; i<wallVoxDim.volume(); ++i)
	{
		wallVoxMap[i].nxtInVox=0;
		wallVoxMap[i].nxtToSame=0;
		wallVoxMap[i].prvInVox=0;
		wallVoxMap[i].wall=0;
	}
	
	if (printUpdates)
		std::cout << "memory allocated successfully, setting default voxel values...\n";
		
	for (i=0; i<voxNum; ++i)
	{
		//voxMap[i].density=drand(0, 1);
		voxMap[i].density=startDensity;
		voxMap[i].cntr=Vctr3<float>(0.5, 0.5, 0.5);
		voxMap[i].vel=startVel*areaScale;
		//voxMap[i].deviation=0;
		//voxMap[i].xSD=2;
		//voxMap[i].ySD=2;
		
		//voxMap[i].lock=0;
		
		incoming[i]=NULL_VOXEL;
		
		voxMapAvg[i].density=0;
		voxMapAvg[i].cntr=Vctr3<float>();
		voxMapAvg[i].vel=Vctr3<float>();
		
		startTotalMass+=voxMap[i].density;
	}
	
	Object * ptr=firstObj;
	
	while (ptr)
	{
		//must be this order
		ptr->setBoundingBox();
		
		ptr->makePointers();
		
		if (calcObjAreaRes)
			ptr->calcDirectionalArea(calcObjAreaRes, startVel*areaScale);
		
		ptr=ptr->nxtObj;
	}
	
	totalMass=startTotalMass;
	escapedMass=0;
	
	if (printUpdates)
		std::cout << "voxels set\n";
	
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	srand(time.tv_nsec);
	
	if (printUpdates)
		std::cout << "simulation initialized\n\n";
}

void FluidSim3::uninit()
{
	Object * ptr=firstObj;
	
	if (printUpdates)
	{
		std::cout << "uninitializing simulation...\n";
	}
	
	while (ptr)
	{
		ptr->clearPointers();
		
		ptr=ptr->nxtObj;
	}
	
	if (voxMap)
		delete[] voxMap;
	
	if (incoming)
		delete[] incoming;
	
	if (voxMapAvg)
		delete[] voxMapAvg;
	
	if (wallVoxMap)
		delete[] wallVoxMap;
	
	voxNum=0;
	voxMap=0;
	incoming=0;
	voxMapAvg=0;
	wallVoxMap=0;
	
	if (printUpdates)
	{
		std::cout << "uninitialization complete...\n";
	}
}

void FluidSim3::resetObjects()
{
	Object * ptr=firstObj;
	
	while (ptr)
	{
		ptr->reset();
		
		ptr=ptr->nxtObj;
	}
}

void FluidSim3::advance()
{
	Vctr3<int> j;
	int i;
	int zo, zyo; //z and y+z offsets
	
	
	///get started
	
	if (printUpdates)
		std::cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << "FluidSim3 starting frame " << frame << "\n";
		
	const int threadNum=FluidSim3::threadNum; //the program refuses to compile in debug mode without this
	
	if (!voxNum)
	{
		std::cerr << "FluidSim3::advance() called without voxMap properly initialized\n";
		return;
	}
	
	std::thread threadAry[threadNum];
	
	
	///call the moveContents() treads
	
	for (i=0; i<threadNum; ++i)
	{
		threadAry[i]=std::thread(callMoveContents, this, i, threadNum);
	}
	
	if (printUpdates)
		std::cout << "moveContents() called on " << threadNum << " threads...\n";
	
	
	///add the inflow
		
	for (j.z=0; j.z<dim.z; ++j.z)
	{
		zo=j.z*dim.y*dim.x;
		
		for (j.y=0; j.y<dim.y; ++j.y)
		{
			zyo=j.y*dim.x+zo;
			
			for (j.x=0; j.x<startVel.x*areaScale; ++j.x)
			{
				addedMass+=inflowDensity;
				incoming[zyo+j.x].density+=inflowDensity;
				incoming[zyo+j.x].vel.x+=startVel.x*areaScale*inflowDensity;
				incoming[zyo+j.x].cntr+=Vctr3<float>(0.5, 0.5, 0.5)*inflowDensity;
			}
		}
	}
	
	/*
	for (j.z=dim.z*0.2; j.z<dim.z*0.3; ++j.z)
	{
		zo=j.z*dim.y*dim.x;
		
		for (j.y=dim.y*0.4; j.y<dim.y*0.5; ++j.y)
		{
			zyo=j.y*dim.x+zo;
			
			for (j.x=0; j.x<4; ++j.x)
			{
				addedMass+=4;
				incoming[zyo+j.x].density+=4;
				incoming[zyo+j.x].vel.x+=1*4;
				incoming[zyo+j.x].cntr+=Vctr3<float>(0.5, 0.5, 0.5)*4;
			}
		}
	}
	
	for (j.z=dim.z*0.7; j.z<dim.z*0.8; ++j.z)
	{
		zo=j.z*dim.y*dim.x;
		
		for (j.y=dim.y*0.4; j.y<dim.y*0.5; ++j.y)
		{
			zyo=j.y*dim.x+zo;
			
			for (j.x=0; j.x<4; ++j.x)
			{
				addedMass+=4;
				incoming[zyo+j.x].density+=4;
				incoming[zyo+j.x].vel.x+=1*4;
				incoming[zyo+j.x].cntr+=Vctr3<float>(0.5, 0.5, 0.5)*4;
			}
		}
	}
	*/
	
	
	///join the moveContents() threads
	
	for (i=0; i<threadNum; ++i)
	{
		threadAry[i].join();
	}
	
	if (printUpdates)
		std::cout << "moveContents() threads joined\n";
	
	
	///call the applyIncoming() threads
	
	totalMass=0;
	
	for (i=0; i<threadNum; ++i)
	{
		threadAry[i]=std::thread(callapplyIncoming, this, i, threadNum);
	}
	
	if (printUpdates)
		std::cout << "applyIncoming() called on " << threadNum << " threads...\n";
	
	
	///join the applyIncoming() threads
	
	for (i=0; i<threadNum; ++i)
	{
		threadAry[i].join();
	}
	
	if (printUpdates)
		std::cout << "applyIncoming() threads joined\n";
		
	/*
	for (i=0; i<threadNum; ++i)
	{
		callapplyIncoming(this, i, threadNum);
	}
	*/
	
	//callapplyIncoming(this, 0, voxNum);
	
	
	///finish
	
	Object * ptr=firstObj;
	
	while (ptr)
	{
		ptr->advance();
		
		ptr=ptr->nxtObj;
	}
	
	if (printUpdates)
		std::cout << "frame " << frame << " complete\n\n";
		
	++frame;
}

double FluidSim3::moveContents(Vctr3<int> strtLoc, unsigned int * randomSeed)
{
	int i;
	Vctr3<double> vel, pos;
	Vctr3<int> endLoc;
	FluidVoxel * endVox;
	int voxId;
	double escapedMassSum=0;
	double pressureForce=this->pressureForce*areaScale;
	
	if (AXIS_NUM==2)
		voxId=strtLoc.y*dim.x+strtLoc.x;
	else if (AXIS_NUM==3)
		voxId=strtLoc.x+strtLoc.y*dim.x+strtLoc.z*dim.y*dim.x;
		
	FluidVoxel strtVox=voxMap[voxId];
	double dn=strtVox.density; //density, is soon divided by voxDivis
	double exteriorDensity=0; //the sum of the density of surrounding voxels
	//double strtVoxForce=strtVox.density*pressureForce;
	
	if (!dn)
		return 0;
		
	//the density of both the positive and negative direction in all axis's
	Vctr3<double> dnFwd(dn, dn, dn);
	Vctr3<double> dnBkwd(dn, dn, dn);
	
	int voxDivis=ceil(clamp(dn*voxDivisPerDensity, 1, voxDivisMax));
	dn/=voxDivis;
	
	if (!dn)
		return 0; //just in case of an underflow or something
	
	
	///check the density of surrounding particles
	
	if (strtLoc.x>0)
		exteriorDensity+=dnFwd.x=voxMap[voxId-1].density;
		
	if (strtLoc.x<dim.x-1)
		exteriorDensity+=dnBkwd.x=voxMap[voxId+1].density;
		
	if (strtLoc.y>0)
		exteriorDensity+=dnFwd.y=voxMap[voxId-dim.x].density;
		
	if (strtLoc.y<dim.y-1)
		exteriorDensity+=dnBkwd.y=voxMap[voxId+dim.x].density;
		
	if (strtLoc.z>0)
		exteriorDensity+=dnFwd.z=voxMap[voxId-dim.x*dim.y].density;
		
	if (strtLoc.z<dim.z-1)
		exteriorDensity+=dnBkwd.z=voxMap[voxId+dim.x*dim.y].density;
		
	Vctr3<double> baseVel(strtVox.vel+(dnFwd-dnBkwd)*pressureForce/dn);
	Vctr3<double> basePos(strtVox.cntr+strtLoc);
	
	double rndm=randomMove*areaScale*dn*(AXIS_NUM*2+1)/(exteriorDensity+dn);
	//the +dn is to stop dividing by 0, there is no scientific reason to use this method and not min() or something else
	
	///run through all the particles
	
	for (i=0; i<voxDivis; ++i)
	{
		vel=baseVel;
		pos=basePos;
		
		//apply randomness
		/*if (AXIS_NUM==2)
		{
			vel.x+=drand(-rndm, rndm, randomSeed);
			vel.y+=drand(-rndm, rndm, randomSeed);
		}
		else if (AXIS_NUM==3)*/
		
		vel.x+=drand(-rndm, rndm, randomSeed);
		vel.y+=drand(-rndm, rndm, randomSeed);
		vel.z+=drand(-rndm, rndm, randomSeed);
		
		//this gives a better distribution but allows for teleporting through walls
		//pos.x+=drand(-0.5, 0.5, randomSeed);
		//pos.y+=drand(-0.5, 0.5, randomSeed);
		//pos.z+=drand(-0.5, 0.5, randomSeed);
		
		bounceOffWalls(&pos, &vel, dn, 0, 0, 0);
		
		//apply the velocity to the position
		pos+=vel;
		
		//find the voxel the particle ended up in
		//must be floor instead of standard int rounding so it does not round particles
		//at -0.5 to voxel 0 and thus double the density of lower and left edge voxels
		endLoc=pos.floor();
		
		//add the particles information to the voxel it went to (could be start voxel)
		if (endLoc.isInside(dim))
		{
			endVox=incoming+(endLoc.z*dim.y+endLoc.y)*dim.x+endLoc.x;
			
			//the lock code is supposed to prevent the very rare case of two threads accessing the same voxel at the same time
			//it obviously is suseptable to endless loops now, there doesn't seem to be a problem so I will leave is commented out
			
			//endVox->lock++;
			
			//while (endVox->lock>1) {}
			
			endVox->density+=dn;
			endVox->cntr+=(pos-endLoc)*dn;
			endVox->vel+=vel*dn;
			
			//endVox->lock--;
		}
		else
		{
			escapedMassSum+=dn;
		}
	}
	
	return escapedMassSum;
}

void FluidSim3::bounceOffWalls(Vctr3<double> * strtPos, Vctr3<double> * vel, const double &mass, double prvCollisionTime, Object::CollisionWall * prvCollisionWall, int recursionLevel)
{
	Object::WallPtr * wallPtr;//the collision wall we are looking at
	Object::CollisionWall * wall; //the collision wall we are looking at
	int i, j;
	Vctr3<int> lowLoc, hghLoc, loc;
	double frstClsnTime=0;
	bool clsnExists=0;
	Vctr3<double> frstClsnPos, frstClsnNormal; //the position of the particle and the normal of the collision wall we first collide with
	Object::CollisionWall * frstClsnWall;
	Vctr3<double> frstClsnWallPos[3];
	XYdbl frstClsnCords=mkXYdbl(0, 0); //where on the triangle the first collision takes place
	//Vctr3<double> lowPos=Vctr3<double>::min(*strtPos, *strtPos+ *vel), hghPos=Vctr3<double>::max(*strtPos, *strtPos+ *vel);
	
	
	///loop through the linked list of walls to find the first collision
	
	lowLoc=(Vctr3<double>::min((*strtPos), (*strtPos)+(*vel))*wallVoxDim/dim).clamp(wallVoxDim);
	hghLoc=(Vctr3<int>::max((*strtPos), (*strtPos)+(*vel))*wallVoxDim/dim+Vctr3<int>(1, 1, 1)).clamp(wallVoxDim);
	
	for (loc.x=lowLoc.x; loc.x<hghLoc.x; ++loc.x)
	{
		for (loc.y=lowLoc.y; loc.y<hghLoc.y; ++loc.y)
		{
			for (loc.z=lowLoc.z; loc.z<hghLoc.z; ++loc.z)
			{
				wallPtr=(wallVoxMap+loc.x+loc.y*wallVoxDim.x+loc.z*wallVoxDim.y*wallVoxDim.x)->nxtInVox;
				
				while (wallPtr)
				{
					wall=wallPtr->wall;
					
					if (wall!=prvCollisionWall)//--> this check is redundant with the wallVoxMap in use --> && Vctr3<double>::touches(lowPos, hghPos, wall->bbLow, wall->bbHgh))
					{
						double eqtnAnswr[3]; //the three roots of the cubic
						int answerNum=0;
						
						if (wall->moving)
						{
							//vars used in calculations, the don't represent anything particular, they are just numbers that repeat in the math
							double x1, x2, x3, x4, x5;
							double y1, y2, y3, y4, y5;
							double z1, z2, z3, z4, z5;
							double a, b, c, d; //the input to the cubic
							
							//these plane vectors represent the angle of the plane, their cross product is a perpendicular line to the plane
							
							Vctr3<double> planeVctr1=wall->vert[1]-wall->vert[0]; //the plane's normal vector
							Vctr3<double> planeVctr2=wall->vert[2]-wall->vert[0];
							
							Vctr3<double> planeVctrDlta1=wall->dlta[1]-wall->dlta[0];
							Vctr3<double> planeVctrDlta2=wall->dlta[2]-wall->dlta[0];
							
							//these values don't represent anything particular, they are just numbers that repeat in the math
							//to find how I calculated them, take a look and '3D intersect cubic.txt' but be warned, there may be mistakes in it that I have fixed here
							
							x1=planeVctrDlta1.y*planeVctrDlta2.z-planeVctrDlta1.z*planeVctrDlta2.y;
							x2=vel->x-wall->dlta[0].x;
							x3=planeVctr1.y*planeVctrDlta2.z+planeVctr2.z*planeVctrDlta1.y-planeVctr1.z*planeVctrDlta2.y-planeVctr2.y*planeVctrDlta1.z;
							x4=planeVctr1.y*planeVctr2.z-planeVctr1.z*planeVctr2.y;
							x5=strtPos->x-wall->vert[0].x;
							
							y1=planeVctrDlta1.z*planeVctrDlta2.x-planeVctrDlta1.x*planeVctrDlta2.z;
							y2=vel->y-wall->dlta[0].y;
							y3=planeVctr1.z*planeVctrDlta2.x+planeVctr2.x*planeVctrDlta1.z-planeVctr1.x*planeVctrDlta2.z-planeVctr2.z*planeVctrDlta1.x;
							y4=planeVctr1.z*planeVctr2.x-planeVctr1.x*planeVctr2.z;
							y5=strtPos->y-wall->vert[0].y;
							
							z1=planeVctrDlta1.x*planeVctrDlta2.y-planeVctrDlta1.y*planeVctrDlta2.x;
							z2=vel->z-wall->dlta[0].z;
							z3=planeVctr1.x*planeVctrDlta2.y+planeVctr2.y*planeVctrDlta1.x-planeVctr1.y*planeVctrDlta2.x-planeVctr2.x*planeVctrDlta1.y;
							z4=planeVctr1.x*planeVctr2.y-planeVctr1.y*planeVctr2.x;
							z5=strtPos->z-wall->vert[0].z;
							
							//find the constants that need to be plugged in to the cubic equation
							
							a=x1*x2+y1*y2+z1*z2;
							b=x3*x2+x1*y5+y3*y2+y1*y5+z3*z2+z1*z5;
							c=x4*x2+x3*x5+y4*y2+y3*y5+z4*z2+z3*z5;
							d=x4*x5+y4*y5+z4*z5;
							
							//use the solveCubic function over in WidapMath to calculate the times of collision between the particle and the plane
							//there could be 0, 1, 2 or 3 times of collision but 1 is most likely
							answerNum=solveCubic(a, b, c, d, eqtnAnswr);
						}
						else
						{
							//vars used in calculations, the don't represent anything particular, they are just numbers that repeat in the math
							double x4, x5;
							double y4, y5;
							double z4, z5;
							double c, d;
							
							//these plane vectors represent the angle of the plane, their cross product is a perpendicular line to the plane
							
							Vctr3<double> planeVctr1=wall->vert[1]-wall->vert[0];
							Vctr3<double> planeVctr2=wall->vert[2]-wall->vert[0];
							
							//these values don't represent anything particular, they are just numbers that repeat in the math
							//to find how I calculated them, take a look and '3D intersect cubic.txt' but be warned, there may be mistakes in it that I have fixed here
							
							x4=planeVctr1.y*planeVctr2.z-planeVctr1.z*planeVctr2.y;
							x5=strtPos->x-wall->vert[0].x;
							
							y4=planeVctr1.z*planeVctr2.x-planeVctr1.x*planeVctr2.z;
							y5=strtPos->y-wall->vert[0].y;
							
							z4=planeVctr1.x*planeVctr2.y-planeVctr1.y*planeVctr2.x;
							z5=strtPos->z-wall->vert[0].z;
							
							c=x4*vel->x+y4*vel->y+z4*vel->z;
							
							if (c)
							{
								d=x4*x5+y4*y5+z4*z5;
								
								answerNum=1;
								eqtnAnswr[0]=-d/c;
							}
						}
						
						//loop through all the times of collision looking for the first one
						for (i=0; i<answerNum; ++i)
						{
							if (eqtnAnswr[i]>=0 && eqtnAnswr[i]<=1 && eqtnAnswr[i]>=frstClsnTime) //if the collision does not take place between time 0 and 1, it doesn't happen this frame and can be ignored
							{
								Vctr3<double> pos; //the position of the particle at the moment of impact
								Vctr3<double> wallPos[3], wallVctr[3]; //the position of vertex 0 of the wall and other wall data at that time
								double time; //the time the collision takes place
								XYdbl cords;
								
								time=eqtnAnswr[i];
								
								pos=(*vel)*time+(*strtPos);
								
								for (j=0; j<3; ++j)
									wallPos[j]=wall->dlta[j]*time+wall->vert[j];
									
								for (j=0; j<3; ++j)
									wallVctr[j]=wallPos[(j+1)%3]-wallPos[j];
								
								cords=mapTriCords(pos, wallPos);
								
								if (cords.x>=0 && cords.x<=1 && cords.y>=0 && cords.y<=1) //if the collision happened with the actual triangle (not the infinitely large plane the triangle is on)
									//wallVctr[0].ang(pos-wallPos[0])<=wallVctr[0].ang(wallPos[2]-wallPos[0]) &&
									//wallVctr[1].ang(pos-wallPos[1])<=wallVctr[1].ang(wallPos[0]-wallPos[1]) &&
									//wallVctr[2].ang(pos-wallPos[2])<=wallVctr[2].ang(wallPos[1]-wallPos[2]))
								{
									clsnExists=1;
									frstClsnNormal=(wallVctr[0].cross(wallVctr[1])).normalized(); //the normal vector of the plane at that time
									frstClsnPos=pos;
									frstClsnWall=wall;
									frstClsnCords=cords;
									frstClsnTime=time;
									for (j=0; j<3; ++j)
										frstClsnWallPos[j]=wallPos[j];
								}
							}
						}
					}
					
					wallPtr=wallPtr->nxtInVox;
				}
			}
		}
	}
	
	if (clsnExists)
	{
		//Vctr3<double> wallVel=frstClsnWall->dlta[0];
		
		//Vctr3<double> rotVctr=(frstClsnWallPos[1]-frstClsnWallPos[0]).inversed()*((frstClsnWall->dlta[1]-wallVel).dot(frstClsnNormal))+
		//					  (frstClsnWallPos[2]-frstClsnWallPos[0]).inversed()*((frstClsnWall->dlta[2]-wallVel).dot(frstClsnNormal));
		
		//double rotSpeed=rotVctr.dst()/2; //the units are in voxels per cycle one voxel away from the axis of rot, not any form of angle
		//Vctr3<double> axisOfRot=rotVctr.cross(frstClsnNormal);//.normalized(); <<may need, may not
		//Vctr3<double> wallVel=frstClsnWall->dlta[1];
		Vctr3<double> wallVel=frstClsnWall->dlta[2]*frstClsnCords.y+(frstClsnWall->dlta[1]*frstClsnCords.x+frstClsnWall->dlta[0]*(1-frstClsnCords.x))*(1-frstClsnCords.y);
		//Vctr3<double> relativeVel=*vel-(wallVel+frstClsnNormal*rotSpeed*(frstClsnPos-frstClsnWallPos[0]).dst());
		//Vctr3<double> relativeVel=*vel-(wallVel+frstClsnNormal*rotSpeed*0);
		Vctr3<double> relativeVel=*vel-wallVel;
		Vctr3<double> velChange=frstClsnNormal*frstClsnNormal.dot(relativeVel)*(-1-wallBounciness);
		*vel=*vel+velChange;
		*strtPos=frstClsnPos-(*vel)*frstClsnTime;
		Vctr3<double> force=velChange*mass;
		frstClsnWall->force[0]-=force*(1-frstClsnCords.x)*(1-frstClsnCords.y);
		frstClsnWall->force[1]-=force*(frstClsnCords.x)*(1-frstClsnCords.y);
		frstClsnWall->force[2]-=force*(frstClsnCords.y);
		
		if (recursionLevel<MAX_BOUNCE_NUM-1)
			bounceOffWalls(strtPos, vel, mass, frstClsnTime, frstClsnWall, recursionLevel+1);
	}
}

double FluidSim3::applyIncoming(int vox)
{
	float dn=incoming[vox].density;
	
	if (dn)
	{
		//totalMass+=dn;
		voxMap[vox].density=dn;
		//voxMap[vox].cntr=Vctr3<float>(0.5, 0.5, 0.5);//incoming[vox].cntr/dn;
		voxMap[vox].cntr=incoming[vox].cntr/dn;
		voxMap[vox].vel=incoming[vox].vel/dn;
		
		voxMapAvg[vox].density+=dn;
		voxMapAvg[vox].vel+=voxMap[vox].vel;
	}
	else
		voxMap[vox]=NULL_VOXEL;
		
	incoming[vox]=NULL_VOXEL;
	
	return dn;
}

void FluidSim3::renderSimp(XYint renderDim)
{
	int x, y, yo; //yo is y-offset aka y*dim.x, it speeds things up from calculating it each time
	Vctr3<double> pos;
	double sum;
	Object::CollisionWall * wall;
	
	
	///make sure everything is initialized properly
	
	if (!voxNum)
		return;
		
	if (img.getDim()!=renderDim)
		img.newImage(renderDim);
		
	//sets up a buffer of doubles
	/*
	if (renderBufferLng!=rndrDim.x*rndrDim.y)
	{
		if (renderBuffer)
			delete[] renderBuffer;
	
		renderBufferLng=rndrDim.x*rndrDim.y;
		renderBuffer=new double[renderBufferLng];
	
		if (!renderBuffer)
		{
			renderBufferLng=0;
			std::cerr << "FluidSim3::render(): renderBuffer failed to initialize\n";
			return;
		}
	}
	
	
	///clear out the buffer
	
	for (i=0; i<renderBufferLng; ++i)
	{
		renderBuffer[i]=0;
	}
	*/
	
	
	for (y=0; y<renderDim.y; ++y)
	{
		yo=y*renderDim.x;
		
		for (x=0; x<renderDim.x; ++x)
		{
			pos.x=(double)x*dim.x/renderDim.x;
			pos.y=(double)y*dim.y/renderDim.y;
			
			sum=0;
			
			for (pos.z=0; pos.z<dim.z; ++pos.z)
			{
				sum+=voxMap[(int)pos.x+(int)pos.y*dim.x+(int)pos.z*dim.x*dim.y].density;
			}
			
			double d=clamp(sum*0.1/dim.z, 0, 1);
			
			img.bits[yo+x]=hsl2rgb(clrHSL((1-d)*255*4, 255, 255));
			
			//double d=clamp(voxMap[ro+x].density/6, 0, 1);
			//double v=clamp(dst(voxMap[ro+x].xVel-startVel.x*areaScale, voxMap[ro+x].yVel-startVel.y*areaScale), 0, 1);
			//double thta=atan2(voxMap[ro+x].yVel-startVel.y*areaScale, voxMap[ro+x].xVel-startVel.x*areaScale);
			
			///simple density map
			//img.bits[ro+x]=bkndClr;
			//blend(img.bits+ro+x, fluidClr, d);
			
			///boolean
			//img.bits[ro+x]=(d>0.7?fluidClr:bkndClr);
			
			///density map with color gradient
			//img.bits[ro+x]=hsl2rgb(clrHSL((1-d)*255*4, 255, 255));
			
			///cool density map with colors
			//img.bits[ro+x]=hsl2rgb(clrHSL((1-d)*255*5, 255, d*255*2));
			
			///maps density and velocity
			//img.bits[ro+x]=hsl2rgb(clrHSL((1-v)*255*4, 255, d*255*2));
			
			///density, velocity and direction
			//img.bits[ro+x]=hsl2rgb(clrHSL(grdnt(thta, 0, 2*PI, 0, 255*6), v*255, d*255));
		}
	}
	
	wall=firstObj->firstWall;
	
	while (wall)
	{
		for (int i=0; i<3; ++i)
			img.line(mkXYint(wall->vert[i].x, wall->vert[i].y), mkXYint(wall->vert[(i+1)%3].x, wall->vert[(i+1)%3].y), 4, theme.wireClr);
		wall=wall->nxtWall;
	}
}

void FluidSim3::render()
{
	const int threadNum=FluidSim3::threadNum; //the program refuses to compile in debug mode without this
	int i;
	
	if (!voxNum)
		return;
		
	if (img.getDim()!=cam->dim)
	{
		img.newImage(cam->dim);
	}
	
	img.clrFill(theme.bkndClr);
	
	if (renderBoundry)
		showBoundary();
	
	if (renderFlowLines)
		showFlowLines();
	
	if (renderWalls)
		showObjects();
	
	triCount=0;
	
	triCount+=scene3D.getTriCount();
	
	scene3D.render();
	scene3D.clearTriList();
	
	if (transScene.getTriCount())
	{
		if (transScene.getRenderImage()->getDim()!=cam->dim)
		{
			transScene.getRenderImage()->newImage(cam->dim);
		}
		
		triCount+=transScene.getTriCount();
		
		transScene.render();
		transScene.clearTriList();
		
		scene3D.zComposite(&transScene, 1);
	}
	
	///render actual fluid with threads
	
	if (renderFluid)
	{
		std::thread threadAry[threadNum];
		
		for (i=0; i<threadNum; ++i)
		{
			threadAry[i]=std::thread(cam->prsp?renderFluidPrsp:renderFluidOrtho, this, i, threadNum);
		}
		
		for (i=0; i<threadNum; ++i)
		{
			threadAry[i].join();
		}
	}
	
}

void FluidSim3::showBoundary()
{
	/*Vctr3<int> farCorner(sin(deg2rad(camRotZ))<0, cos(deg2rad(camRotZ))>0, cos(deg2rad(camRotX))<0);
	
	if (farCorner.x)
		img.text("X+", mapOrthoPoint(Vctr3<int>(dim.x*1.2, dim.y/2, dim.z/2)), axisLableHgh, clr(255, 0, 0), axisLableWght);
	if (farCorner.y)
		img.text("Y+", mapOrthoPoint(Vctr3<int>(dim.y/2, dim.x*1.2, dim.z/2)), axisLableHgh, clr(0, 255, 0), axisLableWght);
	if (farCorner.z)
		img.text("Z+", mapOrthoPoint(Vctr3<int>(dim.y/2, dim.z/2, dim.x*1.2)), axisLableHgh, clr(0, 0, 255), axisLableWght);
		
	img.line(mapOrthoPoint(farCorner*dim), mapOrthoPoint(Vctr3<double>(!farCorner.x, farCorner.y, farCorner.z)*dim), theme.perimThick, theme.perimClr);
	img.line(mapOrthoPoint(farCorner*dim), mapOrthoPoint(Vctr3<double>(farCorner.x, !farCorner.y, farCorner.z)*dim), theme.perimThick, theme.perimClr);
	img.line(mapOrthoPoint(farCorner*dim), mapOrthoPoint(Vctr3<double>(farCorner.x, farCorner.y, !farCorner.z)*dim), theme.perimThick, theme.perimClr);
	*/
	
	double rds=dim.dst()*0.004;
	const int sections=4;
	const int divisions=12;
	RGBpix color=clr(32, 32, 32);
	Vctr3<double>low, hgh;
	
	low=dim/(-2.0);
	hgh=dim/2.0;
	
	scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z), Vctr3<double>(hgh.x, low.y, low.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z), Vctr3<double>(low.x, hgh.y, low.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, low.y, low.z), Vctr3<double>(low.x, low.y, hgh.z), rds, sections, 0, color, true, divisions);
	
	scene3D.addPipe(Vctr3<double>(hgh.x, low.y, low.z), Vctr3<double>(hgh.x, hgh.y, low.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(hgh.x, low.y, low.z), Vctr3<double>(hgh.x, low.y, hgh.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, hgh.y, low.z), Vctr3<double>(hgh.x, hgh.y, low.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, hgh.y, low.z), Vctr3<double>(low.x, hgh.y, hgh.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, low.y, hgh.z), Vctr3<double>(hgh.x, low.y, hgh.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, low.y, hgh.z), Vctr3<double>(low.x, hgh.y, hgh.z), rds, sections, 0, color, true, divisions);
	
	scene3D.addPipe(Vctr3<double>(hgh.x, hgh.y, low.z), Vctr3<double>(hgh.x, hgh.y, hgh.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(hgh.x, low.y, hgh.z), Vctr3<double>(hgh.x, hgh.y, hgh.z), rds, sections, 0, color, true, divisions);
	scene3D.addPipe(Vctr3<double>(low.x, hgh.y, hgh.z), Vctr3<double>(hgh.x, hgh.y, hgh.z), rds, sections, 0, color, true, divisions);
}

void FluidSim3::showObjects()
{
	Object * ptr=firstObj;
	
	while (ptr)
	{
		ptr->addToScene();
		
		ptr=ptr->nxtObj;
	}
}

void FluidSim3::showFlowLines()
{
	const int sections=3; //how many sections each line is made up of (3 is triangular prisms)
	const int sctnsY=4, sctnsZ=4; //the number of lines in the grid
	Vctr3<double> startPos, posPrv, posNxt, vertsPrv[sections], vertsNxt[sections]; //various positions that need to be remembered
	Vctr3<double> ofstZ, ofstX, ofstY; //used in math, I wrote it but I don't really remember what they mean anymore. but hay, it works (for now).
	double ang=deg2rad(360.0/sections), rds; //used in math
	FluidVoxel * vox; //a pointer to the current voxel
	int counter, i, y, z;
	const int MAX_COUNT=256; //max number of segments in each line
	const double SEGMENT_LNG=0.5; //length of a segment
	const double SEGMENT_RDS=0.25; //radius of the lines
	RGBpix color=flowLinesClr;
	
	if (frame<=0)
		return;
	
	for (y=0; y<sctnsY; y++)
	{
		for (z=0; z<sctnsZ; z++)
		{
			startPos=Vctr3<double>(0, grdnt(y, -1, sctnsY, 0, dim.y), grdnt(z, -1, sctnsZ, 0, dim.z));
			
			posPrv=startPos;
			
			for (i=0; i<sections; ++i)
			{
				vertsPrv[i]=startPos-dim/2;
			}
			
			counter=0;
			
			rds=SEGMENT_RDS;
			
			while (posPrv.isInside(dim) && counter<MAX_COUNT)
			{
				vox=&voxMapAvg[(int)posPrv.x+(int)posPrv.y*dim.x+(int)posPrv.z*dim.x*dim.y];
				
				if (vox->density/frame<=0.1 || vox->vel.dstSquared()==0)
					break;
				
				color=hsl2rgb(clrHSL(grdnt(clamp(vox->density/frame, 0, maxDisplayDensity), 0, maxDisplayDensity, 255*4, 0), 255, 255));
				
				//posNxt=posPrv+vox->vel*vox->density*SEGMENT_LNG/(frame*frame);
				posNxt=posPrv+vox->vel.normalized()*SEGMENT_LNG;
				
				//rds=clamp(vox->density*SEGMENT_RDS/frame, 0.1, 4.0);
				
				ofstZ=posNxt-posPrv;
				
				if (!ofstZ.x && !ofstZ.y)
					ofstX=Vctr3<double>::cross(ofstZ, Vctr3<double>(0, 1, 0));
				else
					ofstX=Vctr3<double>::cross(ofstZ, Vctr3<double>(0, 0, 1));
				
				ofstX=ofstX.normalized()*rds;
				
				ofstY=Vctr3<double>::cross(ofstZ, ofstX);
				
				ofstY=ofstY.normalized()*rds;
				
				for (i=0; i<sections; ++i)
				{
					//verts[1]=posNxt+ofstX*cos((i+0.0)*ang)+ofstY*sin((i+0.0)*ang)-dim/2;
					vertsNxt[i]=posNxt+ofstX*cos((i+0.5)*ang)+ofstY*sin((i+0.5)*ang)-dim/2;
					
					//verts[3]=posNxt+ofstX*cos((i+1.0)*ang)+ofstY*sin((i+1.0)*ang)-dim/2;
					vertsNxt[(i+1)%sections]=posNxt+ofstX*cos((i+1.5)*ang)+ofstY*sin((i+1.5)*ang)-dim/2;
					
					transScene.addTriangle(vertsNxt[i], vertsNxt[(i+1)%sections], vertsPrv[i], color, flowLinesShadeless);
					transScene.addTriangle(vertsNxt[(i+1)%sections], vertsPrv[(i+1)%sections], vertsPrv[i], color, flowLinesShadeless);
				}
				
				for (i=0; i<sections; ++i)
				{
					vertsPrv[i]=vertsNxt[i];
				}
				
				posPrv=posNxt;
				
				++counter;
			}
			
		}
	}
}

void FluidSim3::renderFluidOrtho(FluidSim3 * obj, int threadId, int threadNum)
{
	int x, y, yo; //yo is y-offset aka y*dim.x, it speeds things up from calculating it each time
	Vctr3<int> pos;
	double depthScale=obj->dim.dst()/2;
	float * zBuffer=obj->scene3D.getZBuffer();
	float zVal;
	Scene3D::Cam * cam=obj->cam;
	
	///set up the factors
	
	Vctr3<double> depthFactor(
		sin(deg2rad(cam->rot.z))*sin(deg2rad(cam->rot.x)),
		-cos(deg2rad(cam->rot.z))*sin(deg2rad(cam->rot.x)),
		cos(deg2rad(cam->rot.z)));
		
	Vctr3<double> xFactor(
		cos(deg2rad(cam->rot.z))*cam->fov,
		sin(deg2rad(cam->rot.z))*cam->fov,
		0);
		
	Vctr3<double> yFactor(
		-sin(deg2rad(cam->rot.z))*cos(deg2rad(cam->rot.x))*cam->fov,
		cos(deg2rad(cam->rot.z))*cos(deg2rad(cam->rot.x))*cam->fov,
		sin(deg2rad(cam->rot.x))*cam->fov);
		
	Vctr3<double> offset;
	
	double depth;
	double sum, val;
	
	int low=threadId*cam->dim.y/threadNum;
	int hgh=(threadId+1)*cam->dim.y/threadNum;
	
	
	///run through all the pixels
	
	for (y=low; y<hgh; ++y)
	{
		yo=y*cam->dim.x;
		
		for (x=0; x<cam->dim.x; ++x)
		{
			sum=0;
			
			offset=xFactor*(x-cam->dim.x/2)+yFactor*(y-cam->dim.y/2)+obj->dim/2;
			
			if (zBuffer)
				zVal=std::max(-zBuffer[x+yo], -(float)depthScale);
			else
				zVal=-depthScale;
			
			//for (depth=-depthScale; depth<depthScale; ++depth)
			for (depth=zVal; depth<depthScale; ++depth)
			//for (depth=-depthScale; depth<0; ++depth)
			{
				pos=depthFactor*depth+offset;
				
				if (pos.isInside(obj->dim))
				{
					val=obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].density;
					//val=obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].density*(obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].vel-obj->startVel*areaScale).dst();
					
					//if (val>6)
					sum+=val;
				}
			}
			
			double dn;
			//dn=clamp(sum*0.5/dim.z, 0, 1);
			
			dn=clamp(1-1/(sum*1.0/obj->dim.z+1), 0, 1);
			
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-d)*255*4, 255, 255));
			
			//double d=clamp(voxMap[yo+x].density/6, 0, 1);
			//double v=clamp(dst(voxMap[ro+x].xVel-startVel.x*areaScale, voxMap[ro+x].yVel-startVel.y*areaScale), 0, 1);
			//double thta=atan2(voxMap[ro+x].yVel-startVel.y*areaScale, voxMap[ro+x].xVel-startVel.x*areaScale);
			
			///simple density map
			blend(obj->img.bits+yo+x, obj->theme.fluidClr, dn);
			
			///boolean
			//if (dn>0)
			//	obj->img.bits[yo+x]=(obj->theme.fluidClr);
			
			///density map with color gradient
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-dn)*255*4, 255, 255));
			
			///cool density map with colors
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-d)*255*5-255, 255, d*255*2));
			
			///maps density and velocity
			//img.bits[ro+x]=hsl2rgb(clrHSL((1-v)*255*4, 255, d*255*2));
			
			///density, velocity and direction
			//img.bits[ro+x]=hsl2rgb(clrHSL(grdnt(thta, 0, 2*PI, 0, 255*6), v*255, d*255));
		}
	}
}

void FluidSim3::renderFluidPrsp(FluidSim3 * obj, int threadId, int threadNum)
{
	int x, y, yo; //yo is y-offset aka y*dim.x, it speeds things up from calculating it each time
	Vctr3<int> pos;
	float * zBuffer=obj->scene3D.getZBuffer();
	float zVal;
	Scene3D::Cam * cam=obj->cam;
	Vctr3<double> depthFactor;
	Vctr3<double> camPos=(Vctr3<double>)obj->dim/2.0+cam->pos;
	double near=std::max(cam->pos.dst()-obj->dim.dst()/2, 0.0), far=std::max(cam->pos.dst()+obj->dim.dst()/2, 0.0);
	
	double depth;
	double sum, val;
	
	int low=threadId*cam->dim.y/threadNum;
	int hgh=(threadId+1)*cam->dim.y/threadNum;
	
	double ang, lng;
	
	///run through all the pixels
	
	for (y=low; y<hgh; ++y)
	{
		yo=y*cam->dim.x;
		
		for (x=0; x<cam->dim.x; ++x)
		{
			sum=0;
			
			depthFactor=Vctr3<double>(
				sin(deg2rad((x-cam->dim.x/2)*cam->fov/cam->dim.x)),
				cos(deg2rad((x-cam->dim.x/2)*cam->fov/cam->dim.x))*cos(deg2rad((y-cam->dim.y/2)*cam->fov/cam->dim.x)),
				sin(deg2rad((y-cam->dim.y/2)*cam->fov/cam->dim.x)));
			
			ang=atan2(depthFactor.z, depthFactor.y)+deg2rad(cam->rot.x-90);
			lng=dst(depthFactor.z, depthFactor.y);
			
			depthFactor.y=cos(ang)*lng;
			depthFactor.z=sin(ang)*lng;
			
			ang=atan2(depthFactor.y, depthFactor.x)+deg2rad(cam->rot.z);
			lng=dst(depthFactor.y, depthFactor.x);
			
			depthFactor.x=cos(ang)*lng;
			depthFactor.y=sin(ang)*lng;
			
			if (zBuffer)
				zVal=std::min(zBuffer[x+yo], (float)far);
				//zVal=zBuffer[x+yo]<INFINITY?0:far;
			else
				zVal=far;
			
			//for (depth=-depthScale; depth<depthScale; ++depth)
			for (depth=near; depth<zVal; ++depth)
			//for (depth=-depthScale; depth<0; ++depth)
			{
				pos=(camPos+depthFactor*depth).floor();
				//pos=depthFactor*depth+cam->pos+obj->dim/2;
				
				if (pos.isInside(obj->dim))
				{
					val=obj->voxMapAvg[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].density/obj->frame;
					//val=obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].density;
					
					//val=obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].density*(obj->voxMap[(pos.z*obj->dim.y+pos.y)*obj->dim.x+pos.x].vel-obj->startVel*areaScale).dst();
					
					if (val>1)
						sum+=val-1;
				}
			}
			
			double dn;
			//dn=clamp(sum*0.5/dim.z, 0, 1);
			
			dn=clamp(1-1/(sum*1.0/obj->dim.z+1), 0, 1);
			
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-d)*255*4, 255, 255));
			
			//double d=clamp(voxMap[yo+x].density/6, 0, 1);
			//double v=clamp(dst(voxMap[ro+x].xVel-startVel.x*areaScale, voxMap[ro+x].yVel-startVel.y*areaScale), 0, 1);
			//double thta=atan2(voxMap[ro+x].yVel-startVel.y*areaScale, voxMap[ro+x].xVel-startVel.x*areaScale);
			
			///simple density map
			blend(obj->img.bits+yo+x, obj->theme.fluidClr, dn);
			
			///boolean
			//if (dn>0)
			//	obj->img.bits[yo+x]=(obj->theme.fluidClr)
			
			///density map with color gradient
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-dn)*255*4, 255, 255));
			
			///cool density map with colors
			//img.bits[yo+x]=hsl2rgb(clrHSL((1-d)*255*5-255, 255, d*255*2));
			
			///maps density and velocity
			//img.bits[ro+x]=hsl2rgb(clrHSL((1-v)*255*4, 255, d*255*2));
			
			///density, velocity and direction
			//img.bits[ro+x]=hsl2rgb(clrHSL(grdnt(thta, 0, 2*PI, 0, 255*6), v*255, d*255));
		}
	}
}

void FluidSim3::callMoveContents(FluidSim3 * obj, int threadId, int threadNum)
{
	Vctr3<int> dim;
	Vctr3<int> loc;
	double escapedMassSum=0;
	unsigned int randomSeed=(unsigned int)rand()/2;
	
	dim=obj->getDim();
	
	for (loc.z=threadId*dim.z/threadNum; loc.z<(threadId+1)*dim.z/threadNum; ++loc.z)
	{
		for (loc.y=0; loc.y<dim.y; ++loc.y)
		{
			for (loc.x=0; loc.x<dim.x; ++loc.x)
			{
				escapedMassSum+=obj->moveContents(loc, &randomSeed);
			}
		}
	}
	
	obj->areaDataMutex.lock();
	
	obj->escapedMass+=escapedMassSum;
	
	obj->areaDataMutex.unlock();
}

void FluidSim3::callapplyIncoming(FluidSim3 * obj, int threadId, int threadNum)
{
	int i;
	int voxNum=obj->getDim().volume();
	double massSum=0;
	
	for (i=threadId*voxNum/threadNum; i<(threadId+1)*voxNum/threadNum; ++i)
	{
		massSum+=obj->applyIncoming(i);
	}
	
	obj->areaDataMutex.lock();
	
	obj->totalMass+=massSum;
	
	obj->areaDataMutex.unlock();
}


