#pragma once

#include "../../Widap_includes/WidapStd.h"
#include "../../Widap_includes/WidapImage.h"
#include "../../Widap_includes/Scene3D.h"
#include "../../Widap_includes/Vctr3.h"
#include <mutex>

const char PROG_NAME[]="FluidSim v4.0";

class FluidSim3
{
public:
	
	class Object
	{
	public:
		struct WallPtr;
		
		struct CollisionWall
		{ //describes a single triangle that fluid can collide with
			Vctr3<double> vert[3]; //the vertices of the triangle
			Vctr3<double> dlta[3]; //the movement of the vertices of the triangle
			Vctr3<double> force[3]; //the force of fluid on each vert
			Vctr3<double> totalForce[3]; //the force of fluid on each vert since this object has been last reset
			Vctr3<double> bbLow, bbHgh; //the bounding box
			double area;
			bool moving; //if this wall is moving (may currently be broken if true, all the math works but I haven't tested with movement for a while)
			CollisionWall * nxtWall; //for the linked list
			WallPtr * ptrListStrt; //the start of the list of pointers to this wall (follow nxtToSame until is is null to get full list)
		};
		
		struct WallPtr
		{ //a unit in the linked mesh (lists that interlock) for finding collision walls near a location
			
			CollisionWall * wall; //the wall that this points to
			WallPtr * nxtInVox; //the next wall pointer in this voxel
			WallPtr * prvInVox; //the previous wall pointer in this voxel, if null then this WallPtr is in the 3D array and should not be deleted
			WallPtr * nxtToSame; //the next in the list that all point to the same wall
		};
		
		Object(FluidSim3 * sim, const char * objName=0, RGBpix objClr={0, 0, 0});
		~Object();
		
		void addWall(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2);
		
		void reset(); //reset values
		void advance();
		void addToScene();
		
		void clearPointers(); //clear all the pointers to walls in this object
		void makePointers(); //make pointers for the voxel map linked list
		void setBoundingBox(); //sets the bounding box for the object and all of its walls along the way
		
		void calcDirectionalArea(int dim, Vctr3<double> direction);
		
		char name[256];
		
		int getFramesSinceReset() {return framesSinceReset;}
		
		int polyCount;
		Vctr3<double> bbLow, bbHgh; //the bounding box for the whole object
		Vctr3<double> totalDrag;
		double directionalArea; //the surface area projected from the direction directionalAreaVctr, made with a raster approximation
		Vctr3<double> directionalAreaVctr;
		double dragCoefficient; //the force divided by the directional area
		
		Object * nxtObj, * prvObj; //used to form a linked list of objects
		
		CollisionWall * firstWall; //pointer to first wall (aka triangle)
		
		bool showWalls, showBB, showData; //if to display the various properties
		double showBBExpand; //how much to expand the bounding box when displayed (proportional to object size)
		double showBBThick; //the thickness of the bounding box when displayed
		double showDataSize; //the text height
		RGBpix wallClr; //the color it will be rendered (actually it doesn't really do anything because of colored force collor picking)
		RGBpix bbClr, dataClr; //the color they will be rendered
		
	private:
		
		FluidSim3 * sim;
		int framesSinceReset;
	};
	
	struct FluidVoxel
	{
		float density; //amount of fluid in the voxel
		Vctr3<float>cntr; //from 0 to 1, the center of mass of the voxel
		Vctr3<float>vel; //average velocity
		//float deviation; //velocity deviation
		//float xSD, ySD; //velocity standard deviation

		//char lock; //used to lock the voxel from being changed by multiple threads at once
	};
	
	struct RenderTheme
	{
		RGBpix bkndClr; //background color
		RGBpix fluidClr; //color of the fluid or gas

		RGBpix wireClr; //color of collision walls wire frame
		int wireThick; //thickness of the walls wire frame

		RGBpix perimClr; //color of the wire frame outline around the area
		int perimThick; //thickness of that outline
	};
	
	FluidSim3();
	~FluidSim3();
	
	void setDim(Vctr3<int> newDim, double newAreaScale); //note: this does not init simulation, it uninits it if it is already inited; whats that spell check? init isn't a word, much less uninits and inited? well, they ought to be.
	
	Vctr3<int> getDim()
	{
		return dim;
	}
	int getWdth()
	{
		return dim.x;
	}
	int getHght()
	{
		return dim.y;
	}
	
	void loadStl(const char * filename, Vctr3<double> center, double scale, RGBpix color); //scale is in voxels per stl unit
	void addBoundaryObj(bool xNeg, bool xPos, bool yNeg, bool yPos, bool zNeg, bool zPos); //adds an object containing walls on the specified boundary's of the area
	
	void init(bool ignoreLowRam=0);
	void uninit(); //frees memory, if your planning to immediately reinit, just calling init will do that
	
	void resetObjects();
	
	void advance();

	//must be public so thread function can call it
	double moveContents(Vctr3<int> loc, unsigned int * seed);
	double applyIncoming(int vox);

	void renderSimp(XYint rndrDim);

	void render();

	int frame;
	WidapImage img; //the image on which the sim will be rendered
	
	
	///settings
	
	bool printUpdates; //if to cout updates at the beginning and a few times each frame
	
	float startDensity; //density to start every voxel at
	float inflowDensity; //the density of the incoming fluid
	Vctr3<float> startVel; //initial velocity
	
	double voxDivisPerDensity; //how many particles to divide each density unit into
	int voxDivisMax; //max number of particles to divide the box into
	
	double randomMove; //randomness to add to the velocity
	double pressureForce; //force of pressure
	double wallBounciness; //how bouncy walls are
	
	RenderTheme theme; //the visual theme for how to render, not really used much with the new renderer
	bool renderWalls, renderFluid; //if to render the various elements
	int calcObjAreaRes; //the resolution at which to calculate object directional area, if 0 (default) area will not be automatically calculated
	double maxWallForce; //only affects rendering; the highest wall force that can be displayed
	
	///stats
	
	long double startTotalMass;
	long double totalMass;
	long double addedMass; //inflow
	long double escapedMass; //mass that has left the edges of the map
	
	std::mutex areaDataMutex; //gets locked when changing area stats like total mass and stuff
	
	int polyCount; //number of polygons
	
	Scene3D::Cam * cam;
	
private:

	void bounceOffWalls(Vctr3<double> * pos, Vctr3<double> * d, const double &mass, double prvCollisionTime, Object::CollisionWall * prvCollisionWall=0, int recursionLevel=0);

	static void callMoveContents(FluidSim3 * obj, int threadId, int threadNumber);
	static void callapplyIncoming(FluidSim3 * obj, int threadId, int threadNumber);

	///area vars

	FluidVoxel * voxMap; //the data for the simulation area
	FluidVoxel * incoming; //the data coming from other voxels
	Vctr3<int> dim;
	double areaScale;
	double wallVoxDimScale; //what to multiply dim by to get wallVoxDim, actual wallVoxDim should be used most places because that could be set differently in the future
	int voxNum; //dimensions of the area
	static const int AXIS_NUM=3; //must currently be 3, if the sim is 3D or 2D
	
	Object * firstObj;
	Object * boundaryObj; //null by default; if you add in a boundary it will point to that object; boundary object will also be in the normal object list; if you add multiple boundary objects, all but the last will be erased
	
	Object::WallPtr * wallVoxMap; //the 3D array linked lists to collision walls
	Vctr3<int> wallVoxDim; //the size of the wall vox array
	
	
	///render vars
	
	Scene3D scene3D;
	
	
	///const settings
	
	static const int MAX_BOUNCE_NUM=32; //max number of times a particle can bounce
	static const int threadNum=6; //number of threads (multi-threading has issues and is actually slower)
	
	FluidVoxel NULL_VOXEL; //in all caps because it will not be changed after the constructor
	
	
	///render functions
	
	void showBoundary();
	void showObjects();
	static void renderFluidOrtho(FluidSim3 * obj, int threadId, int threadNum); //should be called after any mesh rendering, can be called on many threads at once
	static void renderFluidPrsp(FluidSim3 * obj, int threadId, int threadNum); //should be called after any mesh rendering, can be called on many threads at once
};

