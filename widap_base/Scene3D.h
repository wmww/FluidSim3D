#include "Vctr3.h"
#include "WidapImage.h"

class Scene3D
{
public:
	
	class Cam //camera
	{
	public:
		Cam();
		~Cam();
		
		Vctr3<double> rot; //rotation of cam
		Vctr3<double> pos; //position of cam
		double fov; //field of view
		bool prsp; //if to render perspective (ortho otherwise)
		XYint dim; //the dimensions of the render
	};
	
	struct Triangle
	{
		Vctr3<double> verts[3];
		RGBpix color;
		bool shadeless;
		Triangle * nxt;
	};
	
	Scene3D();
	~Scene3D();
	
	int getTriCount() {return triCount;}
	float * getZBuffer() {return zBuffer;}
	int getZBufferLng() {return zBufferLng;}
	WidapImage * getRenderImage() {return renderImage;}
	
	void setRenderImage(WidapImage * newImg);
	//void setCamRot(double camRotX, double camRotZ);
	//void setCamScale(double newCamScale) {camScale=newCamScale;}
	//void setCamFov(double newCamFov) {camFov=newCamFov;}
	//void setCamPos(Vctr3<double> newCamPos) {camPos=newCamPos;}
	//void setRndrPrsp(int newVal) {rndrPrsp=newVal;}
	
	void addTriangle(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix clr={0, 0, 0}, bool shadeless=0);
	void clearTriList();
	
	//higher level triangle adding
	void addPipe(Vctr3<double> start, Vctr3<double> end, double rds, int sections, int cap, RGBpix color, bool shadeless, int divisions); //make multiple pipes lined up for distorted rendering
	void addPipe(Vctr3<double> start, Vctr3<double> end, double rds, int sections, int cap, RGBpix color, bool shadeless); //cap values: 0 - none, 1 - flat rays, 2 - spike
	void addText(const char * text, Vctr3<double> start, double size, double weight, RGBpix color, bool shadeless);
	
	void render();
	void zComposite(Scene3D * otherScene, double alpha); //composites two scenes together (both must already be rendered an the same size)
	
	Vctr3<double> mapOrthoPoint(Vctr3<double> a); //returned value is in screen cords
	Vctr3<double> mapPrspPoint(Vctr3<double> a); //returned value is in screen cords
	
	void renderTri(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix color); //the verts are in screen cords
	
	Cam cam0;
	Cam * cam; //set to point to cam0 by default, can be changed to a deferent camera, will cause undefined behavior if it does not point to a camera
	
private:
	
	int triCount; //number of triangles in the scene
	Triangle * triStart; //the start of the linked list
	
	//double camRotX, camRotZ, camScale;
	//double camFov;
	//Vctr3<double> camPos;
	//bool rndrPrsp; //if to render perspective (ortho otherwise)
	
	Vctr3<double> lightVctr;
	
	//XYint renderDim;
	WidapImage * renderImage; //the image that is too be rendered to
	float * zBuffer; //the zBuffer
	int zBufferLng; //how much memory is allocated for the z-buffer
};

