#include "../widap_base/WidapStd.h"
#include "../widap_base/WidapImage.h"
#include "../widap_base/WidapSfmlWindow.h"
#include "../widap_base/WidapTimer.h"

#include <unistd.h>

#include "FluidSim3D.h"

const double AREA_SIZE_SCALE=1;
//const int RNDR_X=1280, RNDR_Y=720;
const int RNDR_X=640, RNDR_Y=640;

const RGBpix bkndClr= {64, 64, 64};
WidapSfmlWindow window;

FluidSim3 sim;

bool saveImg=0; //if to save every frame rendered
bool render=1; //if to render each frame

WidapTimer tmr;
int frameOfLastReset=0; //the frame at which the last reset occurred
int frameCounter=0; //frames since timer reset
double fps=0; //the lase calculated fps

bool spinCam=0;
double spinSpeed=2;
bool advanceSim=1; //if to advance the simulation (otherwise it just renders)
double camRotDX=40, camRotDZ=-60, camZoomD=522;
double camRotDecayRate=0.65;
//double camRotDecayRate=0;
//double camRotX=80, camRotZ=10, camZoom=1;
double camRotX=-20, camRotZ=30, camZoom=-1580;
//double camRotX=90, camRotZ=180, camZoom=1;

void init();
void iteration();
void displayData();

int main()
{
	init();
	
	while (window.isOpen())
	{
		iteration();
	}
	
	return 0;
}

void init()
{
	//std::cout << dstToLine(Vctr3<double>(1, 0, 0), Vctr3<double>(0, 2, 0), Vctr3<double>(1, 2, 0)) << "\n";
	
	/*Vctr3<double> vert[3]; //the vertices of the triangle
	Vctr3<double> point(1.5, 0.5, 1); //the vertices of the triangle
	
	vert[0]=Vctr3<double>(1, 0, 1);
	vert[1]=Vctr3<double>(2, 0, 1);
	vert[2]=Vctr3<double>(1.5, 1, 1);
	
	std::cout << mapTriCords(point, vert).x << ", " << mapTriCords(point, vert).y << "\n";
	*/
	
	Vctr3<int> dim(120, 50, 30);
	
	sim.setDim(dim, AREA_SIZE_SCALE);
	
	///window init
	
	window.open(RNDR_X+424, RNDR_Y+24, PROG_NAME);
	
	usleep(40000);
	window.clrFill(bkndClr);
	window.setLoc(mkXY(RNDR_X/2-80, RNDR_Y/2));
	window.text("Loading...", 24, sim.theme.fluidClr);
	window.update();
	
	
	///display settings
	
	FluidSim3::RenderTheme blueprint;
	{
		blueprint.bkndClr=clr(0, 0, 32);
		blueprint.fluidClr=clr(128, 255, 255);
		blueprint.wireClr=clr(255, 255, 255);
		blueprint.wireThick=1;
		blueprint.perimClr=clr(0, 0, 0);
		blueprint.perimThick=6;
	}
	
	FluidSim3::RenderTheme blackGreen;
	{
		blackGreen.bkndClr=clr(0, 0, 0);
		blackGreen.fluidClr=clr(0, 255, 0);
		blackGreen.wireClr=clr(255, 255, 255);
		blackGreen.wireThick=3;
		blackGreen.perimClr=clr(0, 255, 255);
		blackGreen.perimThick=8;
	}
	
	FluidSim3::RenderTheme pinkFluid;
	{
		pinkFluid.bkndClr=clr(16, 16, 16);
		pinkFluid.fluidClr=clr(255, 0, 128);
		pinkFluid.wireClr=clr(128, 128, 128);
		pinkFluid.wireThick=3;
		pinkFluid.perimClr=clr(0, 0, 0);
		pinkFluid.perimThick=8;
	}
	
	FluidSim3::RenderTheme grayscale;
	{
		grayscale.bkndClr=clr(64, 64, 64);
		grayscale.fluidClr=clr(0, 255, 255);
		grayscale.wireClr=clr(64, 64, 64);
		grayscale.wireThick=3;
		grayscale.perimClr=clr(0, 255, 255);
		grayscale.perimThick=8;
	}
	
	sim.theme=blackGreen;
	
	sim.flowLinesClr=clr(0, 255, 255);
	sim.flowLinesShadeless=1;
	
	sim.renderBoundry=0;
	sim.renderFluid=0;
	sim.renderWalls=1;
	sim.renderFlowLines=0;
	sim.calcObjAreaRes=640;
	sim.maxWallForce=4.0;
	sim.maxDisplayDensity=2.0;
	
	
	///fluid settings
	
	sim.startDensity=0; //density to start every voxel at
	sim.inflowDensity=1; //density to flow in
	sim.startVel=Vctr3<float>(0.8, 0, 0); //initial velocity
	
	//sim.voxDivisPerDensity=48; //how many particles to divide each density unit into
	//sim.voxDivisMax=120; //max number of particles to divide the box into
	
	sim.voxDivisPerDensity=12; //how many particles to divide each density unit into
	sim.voxDivisMax=48; //max number of particles to divide the box into
	
	sim.randomMove=0.6; //randomness to add to the velocity
	//sim.randomMove=0.005; //randomness to add to the velocity
	//sim.randomMove=0; //randomness to add to the velocity
	sim.pressureForce=0.0002; //force of pressure
	//sim.pressureForce=0.01; //force of pressure
	//sim.pressureForce=0; //force of pressure
	sim.wallBounciness=0.5; //how bouncy walls are
	
	
	///walls
	
	sim.addBoundaryObj(0, 0, 1, 1, 1, 1);
	
	//sim.loadStl("models/wedge.stl", Vctr3<double>(dim.x/2, dim.y/2, 0), dim.z/16.0);
	//sim.loadStl("models/three_objects.stl", Vctr3<double>(dim.x/2, dim.y/2, 0), dim.z*1.1/16.0, clr(0, 0, 255));
	//sim.loadStl("models/cube.stl", Vctr3<double>(dim.x/2, dim.y/2, 0), dim.z*1.1/16.0, clr(0, 0, 255));
	//sim.loadStl("models/tilted cube.stl", Vctr3<double>(dim.x*0.5, dim.y*0.75, dim.z*0.5), 6, clr(0, 255, 0));
	//sim.loadStl("models/sphere_half_64_32.stl", Vctr3<double>(dim.x*0.5, dim.y*0.5, dim.z*0.5), 6, clr(0, 255, 0));
	
	sim.loadStl("models/tesla.stl", Vctr3<double>(dim.x*0.5, dim.y*0.5, dim.z*0.0), 2, clr(255, 255, 255));
	
	//three object in a row
	//sim.loadStl("models/tesla.stl", Vctr3<double>(dim.x*0.5, dim.y*0.5, dim.z*0.3), 1, clr(128, 0, 255));
	//sim.loadStl("models/straight cube.stl", Vctr3<double>(dim.x*0.5, dim.y*0.25, dim.z*0.5), 6, clr(255, 128, 0));
	//sim.loadStl("models/sphere_64_32.stl", Vctr3<double>(dim.x*0.5, dim.y*0.5, dim.z*0.5), 12, clr(0, 255, 128));
	
	//sim.addCollisionWall(Vctr3<double>(0.6, 1.1, 1.1)*dim, Vctr3<double>(0.5, 1.1, 0.0)*dim, Vctr3<double>(0.5, 0.3, 0.0)*dim);
	//sim.addCollisionWall(Vctr3<double>(1.4, 0.6, 0.6)*dim, Vctr3<double>(1.7, 0.7, 0.9)*dim, Vctr3<double>(1.8, 0.0, 0.8)*dim);
	
	//sim.addCollisionWall(Vctr3<double>(1.4, 0, 0)*dim, Vctr3<double>(1.4, 1.0, 0)*dim, Vctr3<double>(1.8, 0, 0.4)*dim);
	//sim.addCollisionWall(Vctr3<double>(1.8, 1.0, 0.4)*dim, Vctr3<double>(1.4, 1.0, 0)*dim, Vctr3<double>(1.8, 0, 0.4)*dim);
	
	//sim.addCollisionWall(Vctr3<double>(0.8, 0.2, 0.3)*dim, Vctr3<double>(0.8, 0.7, 0.2)*dim, Vctr3<double>(0.3, 0.5, 0.9)*dim);
	
	
	///other
	
	sim.init();
	sim.cam->dim=mkXY(RNDR_X, RNDR_Y);
	sim.cam->fov=45;
	sim.cam->prsp=1;
	
	tmr.update(1);
	
	if (!sim.getDim().volume())
	{
		std::cout << "detected that FluidSim failed to init, quiting...\n";
		window.close();
		window.checkInput(); //immediately detect the window closed
	}
	
	tmr.update(1);
}

void iteration()
{
	window.checkInput();
	
	window.clrFill(bkndClr);
	
	if (window.hasFocus())
	{
		while (char key=window.nextKey())
		{
			switch (key)
			{
			case 'a':
				camRotDZ-=2;
				break;
				
			case 'd':
				camRotDZ+=2;
				break;
				
			case 'w':
				camRotDX-=2;
				break;
				
			case 's':
				camRotDX+=2;
				break;
				
			case 'S': //snap or save
				if (window.ctrlDwn())
				{
					saveImg=!saveImg;
				}
				else
				{
					camRotX=((int(camRotX+15))/30)*30;
					camRotZ=((int(camRotZ+15))/30)*30;
					camRotDX=0;
					camRotDZ=0;
				}
				break;
				
			case '=':
				camZoomD+=0.1;
				break;
				
			case '-':
				camZoomD-=0.1;
				break;
				
			case 'R': //rotate or render
				if (window.ctrlDwn())
				{
					render=!render;
				}
				else
				{
					spinCam=!spinCam;
				}
				break;
			
			case 'F': //render fluid
				sim.renderFluid=!sim.renderFluid;
				break;
			
			case 'L': //render flow lines
				sim.renderFlowLines=!sim.renderFlowLines;
				break;
				
			case 'O': //render fluid
				sim.resetObjects();
				break;
				
			case ' ': //start/stop advance sim
				advanceSim=!advanceSim;
				break;
			}
		}
		
		if ((window.lDwn() || window.mDwn() || window.rDwn()) && window.mouseLoc().y>0)
		{
			camRotDZ=-window.mouseDlta().x*0.25;
			camRotDX=window.mouseDlta().y*0.25;
			spinCam=0;
		}
		
		
		//camZoomD+=(1-camRotDecayRate)*camZoom*window.scroll()/10.0;
		camZoomD+=window.scroll()*4;
	}
	
	if (spinCam)
		camRotDZ+=spinSpeed*(1-camRotDecayRate);
		
	camRotZ+=camRotDZ;
	camRotX+=camRotDX;
	camZoom+=camZoomD;
	
	camRotDZ*=camRotDecayRate;
	camRotDX*=camRotDecayRate;
	camZoomD*=camRotDecayRate;
	
	camRotX=clamp(camRotX, 10, 180-10);
	
	while (camRotZ<0)
		camRotZ+=360;
		
	while (camRotZ>=360)
		camRotZ-=360;
		
	//camZoom=clamp(camZoom, 0.01, 50);
	
	if (advanceSim)
		sim.advance();
	
	sim.cam->rot=Vctr3<double>(camRotX, 0, camRotZ);
	sim.cam->pos=Vctr3<double>(-sin(deg2rad(camRotZ))*sin(deg2rad(camRotX))*camZoom, cos(deg2rad(camRotZ))*sin(deg2rad(camRotX))*camZoom, -cos(deg2rad(camRotX))*camZoom);
	
	if (render)
		sim.render();
	
	frameCounter++;
	double time=tmr.checkTime();
	
	if (time>0.5)
	{
		fps=(frameCounter-frameOfLastReset)/time;
		frameOfLastReset=frameCounter;
		tmr.update(1);
	}
	
	displayData();
	
	window.trnsfrFromImg(&sim.img, mkXY(12, 12));
	
	if (saveImg)
	{
		window.text("\nSaving Frame!", 24, clr(0, 0, 255));
		char text[100];
		sprintf(text, "bmp_otpt/img%d", frameCounter);
		sim.img.save(text);
	}
	
	window.display();
}

void displayData()
{
	TextStyle style0, style1;
	char text[400];
	
	style0.height=14;
	style0.weight=1;
	style0.color=clr(0, 255, 0);
	
	style1.height=14;
	style1.weight=2;
	style1.color=clr(0, 0, 255);
	
	window.setLoc(mkXY(sim.img.getWdth()+24, window.getHght()-12));
	sim.img.setLoc(mkXY(20, sim.img.getHght()-10));
	
	sprintf(text, "\nSimulation frame	%d\n",
			sim.frame);
	//sim.img.text(text, style1);
	window.text(text, style0);
	
	sprintf(text, "\nArea dimensions:\nX=%d, Y=%d, Z=%d\n",
			sim.getDim().x, sim.getDim().y, sim.getDim().z);
	//sim.img.text(text, style1);
	window.text(text, style0);
	
	sprintf(text, "\nTriangle Num:	%d\n",
			sim.triCount);
	//sim.img.text(text, style1);
	window.text(text, style0);
	
	sprintf(text, "\nAvg frame Time:		%f\nFPS:			%f\n",
			1/fps, fps);
	//sim.img.text(text, style1);
	window.text(text, style0);
	
	if (sim.frame)
	{
		sprintf(text, "\nStart mass:			%d\nCurrent mass:		%d\nInflow mass/frame:	%d\nEscaped mass/frame:	%d\nRounding error:		%f%%\n",
				(int)sim.startTotalMass, (int)sim.totalMass, (int)sim.addedMass/sim.frame, (int)sim.escapedMass/sim.frame, (double)((sim.startTotalMass-sim.totalMass-sim.escapedMass+sim.addedMass)/(sim.totalMass*100.0)));
		//sim.img.text(text, style1);
		window.text(text, style0);
	}
	
	/*
	sprintf(text, "\nTriangles:		%d\n",
			sim.polyCount);
	//sim.img.text(text, style1);
	window.text(text, style0);
	*/
	
	sprintf(text, "\nView:\nX=%d, Z=%d, Zoom=%d\n",
			(int)camRotX, (int)camRotZ, (int)(camZoom));
	//sim.img.text(text, style1);
	window.text(text, style0);
	
	/*
	sprintf(text, "\n%d\n",
		(int)sizeof(FluidVoxel));
	//sim.img.text(text, style1);
	window.text(text, style0);
	*/
	
	/*if (window.hasFocus())
	{
		if (window.lDwn() || window.mDwn() || window.rDwn())
			sim.img.circle(window.mouse()+mkXY(-12, -12), 10.5, clr(255, 0, 255), 0.5);
		else
			sim.img.circle(window.mouse()+mkXY(-12, -12), 12.5, clr(255, 255, 255), 0.25);
	}*/
}


