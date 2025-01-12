#include "Scene3D.h"
#include <limits.h>

Scene3D::Scene3D()
{
	triCount=0;
	triStart=0;
	
	renderImage=0;
	zBufferLng=0;
	
	//camRotX=90;
	//camRotZ=0;
	//camScale=1;
	//rndrPrsp=1;
	//camFov=60;
	//camPos=Vctr3<double>(0, 0, 0);
	
	cam=&cam0;
	
	lightVctr=Vctr3<double>(1, 1, 1);
}

Scene3D::~Scene3D()
{
	if (zBufferLng)
		delete[] zBuffer;
	
	clearTriList();
}

void Scene3D::setRenderImage(WidapImage * newImg)
{
	renderImage=newImg;
	
	if (!renderImage)
	{
		cam->dim=mkXY(0, 0);
		if (zBufferLng)
			delete[] zBuffer;
		zBufferLng=0;
	}
	else// if (renderImage->getDim()!=cam->dim)
	{
		cam->dim=renderImage->getDim();
		
		if (zBufferLng)
			delete[] zBuffer;
		
		zBufferLng=cam->dim.x*cam->dim.y;
		zBuffer=new float[zBufferLng];
	}
}

void Scene3D::addTriangle(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix color, bool shadeless)
{
	Triangle * ptr=triStart;
	triStart=new Triangle;
	triStart->verts[0]=vert0;
	triStart->verts[1]=vert1;
	triStart->verts[2]=vert2;
	triStart->color=color;
	triStart->shadeless=shadeless;
	triStart->nxt=ptr;
	triCount++;
}

void Scene3D::addPipe(Vctr3<double> start, Vctr3<double> end, double rds, int sections, int cap, RGBpix color, bool shadeless, int divisions)
{
	for (int i=0; i<divisions; i++)
		addPipe((end-start)*(double(i)/divisions)+start, (end-start)*(double(i+1)/divisions)+start, rds, sections, (i>0 && i<divisions-1)?0:cap, color, shadeless);
}

void Scene3D::addPipe(Vctr3<double> start, Vctr3<double> end, double rds, int sections, int cap, RGBpix color, bool shadeless)
{
	Vctr3<double> ofstZ, ofstX, ofstY, ofstTip;
	Vctr3<double> verts[4];
	int i;
	double ang=deg2rad(360.0/sections);
	
	ofstZ=end-start;
	
	ofstTip=ofstZ.normalized()*rds*0.75;
	
	if (!ofstZ.x && !ofstZ.y)
		ofstX=Vctr3<double>::cross(ofstZ, Vctr3<double>(0, 1, 0));
	else
		ofstX=Vctr3<double>::cross(ofstZ, Vctr3<double>(0, 0, 1));
	
	ofstX=ofstX.normalized()*rds;
	
	ofstY=Vctr3<double>::cross(ofstZ, ofstX);
	
	ofstY=ofstY.normalized()*rds;
	
	for (i=0; i<sections; ++i)
	{
		//verts[0]=start+ofstX*cos((i+0.0)*ang)+ofstY*sin((i+0.0)*ang);
		verts[0]=start+ofstX*cos((i+0.5)*ang)+ofstY*sin((i+0.5)*ang);
		
		//verts[1]=end+ofstX*cos((i+0.0)*ang)+ofstY*sin((i+0.0)*ang);
		verts[1]=end+ofstX*cos((i+0.5)*ang)+ofstY*sin((i+0.5)*ang);
		
		//verts[2]=start+ofstX*cos((i+1.0)*ang)+ofstY*sin((i+1.0)*ang);
		verts[2]=start+ofstX*cos((i+1.5)*ang)+ofstY*sin((i+1.5)*ang);
		
		//verts[3]=end+ofstX*cos((i+1.0)*ang)+ofstY*sin((i+1.0)*ang);
		verts[3]=end+ofstX*cos((i+1.5)*ang)+ofstY*sin((i+1.5)*ang);
		
		addTriangle(verts[0], verts[1], verts[2], color, shadeless);
		addTriangle(verts[2], verts[1], verts[3], color, shadeless);
		
		if (cap==1)
		{
			addTriangle(verts[0], verts[2], start, color, shadeless);
			addTriangle(verts[1], verts[3], end, color, shadeless);
		}
		else if (cap==2)
		{
			addTriangle(verts[0], verts[2], start-ofstTip, color, shadeless);
			addTriangle(verts[1], verts[3], end+ofstTip, color, shadeless);
		}
	}
}

void Scene3D::addText(const char * text, Vctr3<double> start, double size, double weight, RGBpix color, bool shadeless)
{
	Vctr3<double> loc=start;
	int i=0, j;
	double lineThick, pixWde; //thickness of the lines and number of pixels each char is wide
	const int sections=2; //4 will make it lines square, 2 will be flat and render fastest
	
	Vctr3<double> a=start, b=start;
	
	lineThick=weight*size/16.0;
	pixWde=size*0.5;
	
	while (text[i])
	{
		if (text[i]=='\n')
		{
			loc.z-=size*1.5;
			loc.y=start.y;
		}
		/*else if (text[i]== '	')
		{
			loc.y=(int)((loc.y-start.y)/(pixWde*6)+1)*6*pixWde+start.y;
		}*/
		else
		{
			for (j=0; j<WidapImage::widapFont[(int)text[i]].lineNum; ++j)
			{
				a.y=loc.y-WidapImage::widapFont[(int)text[i]].lines[0][j].x*pixWde;
				a.z=WidapImage::widapFont[(int)text[i]].lines[0][j].y*size+loc.z;
				b.y=loc.y-WidapImage::widapFont[(int)text[i]].lines[1][j].x*pixWde;
				b.z=WidapImage::widapFont[(int)text[i]].lines[1][j].y*size+loc.z;
				addPipe(a, b, lineThick, sections, 2, color, shadeless);
			}
			
			loc.y-=pixWde*1.5;
		}
		
		++i;
	}
}

void Scene3D::clearTriList()
{
	Triangle * ptr;
	int counter=0;
	
	while (triStart)
	{
		ptr=triStart->nxt;
		delete triStart;
		triStart=ptr;
		++counter;
	}
	
	if (counter!=triCount)
	{
		std::cerr << "Scene3D::clearTriList(): when clearing tri list, there were " << counter << " triangles but triCount was " << triCount << "\n";
	}
	
	triCount=0;
}

void Scene3D::render()
{
	int i; //yo is y-offset, used for speeding loops up
	Triangle * tri;
	double brightness;
	RGBpix color;
	
	if (!cam)
		cam=&cam0;
	
	if (!renderImage)
		return;
	else if (renderImage->getDim()!=cam->dim || cam->dim.x*cam->dim.y!=zBufferLng)
		setRenderImage(renderImage); //resets various things
	
	if (!(cam->dim.x*cam->dim.y))
		return;
	
	//set the light to move with the camera
	lightVctr=Vctr3<double>(sin(deg2rad(cam->rot.x))*sin(deg2rad(cam->rot.z)), -sin(deg2rad(cam->rot.x))*cos(deg2rad(cam->rot.z)), cos(deg2rad(cam->rot.x)));
	
	for (i=0; i<zBufferLng; i++)
	{
		zBuffer[i]=INFINITY;
	}
	
	tri=triStart;
	
	while (tri)
	{
		if (tri->verts[0]==tri->verts[1] || tri->verts[1]==tri->verts[2] || tri->verts[2]==tri->verts[0])
		{
			
		}
		else
		{
			if (tri->shadeless)
			{ //make shadeless
				
				color=tri->color;
			}
			
			else
			{ //shade based on the light
				
				brightness=clamp(grdnt(fabs(rad2deg(Vctr3<double>::cross(tri->verts[1]-tri->verts[0], tri->verts[2]-tri->verts[0]).ang(lightVctr))-90), -45, 90, 0, 1), 0, 1);
				color.r=tri->color.r*brightness;
				color.g=tri->color.g*brightness;
				color.b=tri->color.b*brightness;
			}
			
			
			//renderImage->triangle(mapOrthoPoint(tri->verts[0]), mapOrthoPoint(tri->verts[1]), mapOrthoPoint(tri->verts[2]), tri->color, 0.5);
			
			if (cam->prsp)
				renderTri(mapPrspPoint(tri->verts[0]), mapPrspPoint(tri->verts[1]), mapPrspPoint(tri->verts[2]), color);
			else
				renderTri(mapOrthoPoint(tri->verts[0]), mapOrthoPoint(tri->verts[1]), mapOrthoPoint(tri->verts[2]), color);
		}
		
		tri=tri->nxt;
	}
}

void Scene3D::zComposite(Scene3D * otherScene, double alpha)
{
	WidapImage * othrImg=otherScene->getRenderImage();
	float * othrZBuffer=otherScene->getZBuffer();
	int i;
	
	if (!renderImage || !othrImg || renderImage->getWdth()*renderImage->getHght()!=zBufferLng || zBufferLng!=otherScene->getZBufferLng())
		return;
	
	if (alpha==1)
	{
		for (i=0; i<zBufferLng; ++i)
		{
			if (othrZBuffer[i]<zBuffer[i])
			{
				renderImage->bits[i]=othrImg->bits[i];
				zBuffer[i]=othrZBuffer[i];
			}
		}
	}
	else if (alpha==0) //add
	{
		for (i=0; i<zBufferLng; ++i)
		{
			if (othrZBuffer[i]<zBuffer[i])
			{
				renderImage->bits[i].r=clamp(renderImage->bits[i].r+othrImg->bits[i].r, 0, 255);
				renderImage->bits[i].g=clamp(renderImage->bits[i].g+othrImg->bits[i].g, 0, 255);
				renderImage->bits[i].b=clamp(renderImage->bits[i].b+othrImg->bits[i].b, 0, 255);
				zBuffer[i]=othrZBuffer[i];
			}
		}
	}
	else
	{
		for (i=0; i<zBufferLng; ++i)
		{
			if (othrZBuffer[i]<zBuffer[i])
			{
				blend(renderImage->bits+i, othrImg->bits[i], alpha);
				zBuffer[i]=othrZBuffer[i];
			}
		}
	}
}

Vctr3<double> Scene3D::mapOrthoPoint(Vctr3<double> a)
{
	Vctr3<double> b;
	
	b.x=a.x*cos(deg2rad(cam->rot.z))+a.y*sin(deg2rad(cam->rot.z));
	b.y=-a.x*sin(deg2rad(cam->rot.z))*cos(deg2rad(cam->rot.x))+a.y*cos(deg2rad(cam->rot.z))*cos(deg2rad(cam->rot.x))+a.z*sin(deg2rad(cam->rot.x));
	b.z=1000-a.x*sin(deg2rad(cam->rot.z))*sin(deg2rad(cam->rot.x))+a.y*cos(deg2rad(cam->rot.z))*sin(deg2rad(cam->rot.x))-a.z*cos(deg2rad(cam->rot.x));
	
	b.x=b.x*cam->fov;
	b.y=b.y*cam->fov;
	
	b.x+=cam->dim.x/2;
	b.y+=cam->dim.y/2;
	
	return b;
}

Vctr3<double> Scene3D::mapPrspPoint(Vctr3<double> a)
{
	Vctr3<double> b;
	double ang, lng;
	
	a-=cam->pos;
	
	ang=atan2(a.y, a.x)-deg2rad(cam->rot.z);
	lng=dst(a.y, a.x);
	
	a.x=cos(ang)*lng;
	a.y=sin(ang)*lng;
	
	ang=atan2(a.z, a.y)-deg2rad(cam->rot.x-90);
	lng=dst(a.z, a.y);
	
	a.y=cos(ang)*lng;
	a.z=sin(ang)*lng;
	
	b.z=a.y;
	//b.z=a.dst();
	
	b.x=rad2deg(atan2(a.x, dst(a.y, a.z)))*cam->dim.x/cam->fov;
	b.y=rad2deg(atan2(a.z, dst(a.y, a.x)))*cam->dim.x/cam->fov;
	
	//b.x=rad2deg(atan2(a.x, a.y))*cam->dim.x/fov;
	//b.y=rad2deg(atan2(a.z, a.y))*cam->dim.x/fov;
	
	//b.x=a.x*cam->dim.x/a.y;
	//b.y=a.z*cam->dim.y/a.y;
	
	b.x+=cam->dim.x/2;
	b.y+=cam->dim.y/2;
	
	return b;
}

void Scene3D::renderTri(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix color)
{
	Vctr3<double> verts[3];
	int x, y;
	int xLow, xHgh;
	int yLow, yHgh;
	bool middleOnLeft;
	int yo; //the y offset in the pixmap
	double xm, ym, zo, z; //x and y slope and z-offset
	//const double zFctr=-4;
	
	if (vert0.z<=0 && vert1.z<=0 && vert2.z<=0)
		return;
	
	//set the array to the verts sorted from bottom to top
	if (vert0.y<vert1.y)
	{
		if (vert0.y<vert2.y)
		{
			verts[0]=vert0;
			
			if (vert1.y<vert2.y)
			{
				verts[1]=vert1;
				verts[2]=vert2;
			}
			else
			{
				verts[1]=vert2;
				verts[2]=vert1;
			}
		}
		else
		{
			verts[0]=vert2;
			verts[1]=vert0;
			verts[2]=vert1;
		}
	}
	else
	{
		if (vert1.y<vert2.y)
		{
			verts[0]=vert1;
			
			if (vert0.y<vert2.y)
			{
				verts[1]=vert0;
				verts[2]=vert2;
			}
			else
			{
				verts[1]=vert2;
				verts[2]=vert0;
			}
		}
		else
		{
			verts[0]=vert2;
			verts[1]=vert1;
			verts[2]=vert0;
		}
	}
	
	middleOnLeft=verts[1].x<grdnt(verts[1].y, verts[0].y, verts[2].y, verts[0].x, verts[2].x);
	
	if (verts[1].y==verts[2].y)
		xm=(verts[2].z-grdnt(verts[2].y, verts[0].y, verts[1].y, verts[0].z, verts[1].z))/(verts[2].x-grdnt(verts[2].y, verts[0].y, verts[1].y, verts[0].x, verts[1].x));
	else
		xm=(verts[0].z-grdnt(verts[0].y, verts[1].y, verts[2].y, verts[1].z, verts[2].z))/(verts[0].x-grdnt(verts[0].y, verts[1].y, verts[2].y, verts[1].x, verts[2].x));
		
	if (verts[1].x==verts[2].x)
		ym=(verts[2].z-grdnt(verts[2].x, verts[0].x, verts[1].x, verts[0].z, verts[1].z))/(verts[2].y-grdnt(verts[2].x, verts[0].x, verts[1].x, verts[0].y, verts[1].y));
	else
		ym=(verts[0].z-grdnt(verts[0].x, verts[1].x, verts[2].x, verts[1].z, verts[2].z))/(verts[0].y-grdnt(verts[0].x, verts[1].x, verts[2].x, verts[1].y, verts[2].y));
	
	zo=verts[0].z-verts[0].y*ym-verts[0].x*xm;
	
	yLow=clamp(ceil(verts[0].y), 0, cam->dim.y);
	yHgh=clamp(floor(verts[1].y)+1, 0, cam->dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		xLow=clamp(ceil(grdnt(y, verts[0].y, verts[2-middleOnLeft].y, verts[0].x, verts[2-middleOnLeft].x)), 0, cam->dim.x);
		xHgh=clamp(floor(grdnt(y, verts[0].y, verts[1+middleOnLeft].y, verts[0].x, verts[1+middleOnLeft].x))+1, 0, cam->dim.x);
		
		//xLow=clamp(grdnt(y, verts[0].y, verts[2-middleOnLeft].y, verts[0].x, verts[2-middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, verts[0].y, verts[1+middleOnLeft].y, verts[0].x, verts[1+middleOnLeft].x), 0, cam->dim.x);
		
		yo=(int)y*cam->dim.x;
		
		for (x=xLow; x<xHgh; ++x)
		{
			z=(x)*xm+(y)*ym+zo;
			
			if (z>0 && z<zBuffer[(int)x+yo])
			{
				renderImage->bits[(int)x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[(int)x+yo]=z;
			}
		}
	}
	
	yLow=yHgh;
	yHgh=clamp(floor(verts[2].y)+1, 0, cam->dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		xLow=clamp(ceil(grdnt(y, verts[2].y, verts[0+middleOnLeft].y, verts[2].x, verts[0+middleOnLeft].x)), 0, cam->dim.x);
		xHgh=clamp(floor(grdnt(y, verts[2].y, verts[1-middleOnLeft].y, verts[2].x, verts[1-middleOnLeft].x))+1, 0, cam->dim.x);
		
		//xLow=clamp(grdnt(y, verts[2].y, verts[0+middleOnLeft].y, verts[2].x, verts[0+middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, verts[2].y, verts[1-middleOnLeft].y, verts[2].x, verts[1-middleOnLeft].x), 0, cam->dim.x);
		
		yo=(int)y*cam->dim.x;
		
		for (x=xLow; x<xHgh; ++x)
		{
			z=(x)*xm+(y)*ym+zo;
			
			if (z>0 && z<zBuffer[(int)x+yo])
			{
				renderImage->bits[(int)x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[(int)x+yo]=z;
			}
		}
	}
}

// | | tri rendering with rounding errors
// V V

/*
void Scene3D::renderTri(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix color)
{
	Vctr3<double> verts[3];
	int x, y;
	int xLow, xHgh;
	int yLow, yHgh;
	bool middleOnLeft;
	int yo; //the y offset in the pixmap
	double xm, ym, zo, z; //x and y slope and z-offset
	//const double zFctr=-4;
	
	//set the array to the verts sorted from bottom to top
	if (vert0.y<vert1.y)
	{
		if (vert0.y<vert2.y)
		{
			verts[0]=vert0;
			
			if (vert1.y<vert2.y)
			{
				verts[1]=vert1;
				verts[2]=vert2;
			}
			else
			{
				verts[1]=vert2;
				verts[2]=vert1;
			}
		}
		else
		{
			verts[0]=vert2;
			verts[1]=vert0;
			verts[2]=vert1;
		}
	}
	else
	{
		if (vert1.y<vert2.y)
		{
			verts[0]=vert1;
			
			if (vert0.y<vert2.y)
			{
				verts[1]=vert0;
				verts[2]=vert2;
			}
			else
			{
				verts[1]=vert2;
				verts[2]=vert0;
			}
		}
		else
		{
			verts[0]=vert2;
			verts[1]=vert1;
			verts[2]=vert0;
		}
	}
	
	middleOnLeft=verts[1].x<grdnt(verts[1].y, verts[0].y, verts[2].y, verts[0].x, verts[2].x);
	
	if (verts[1].y==verts[2].y)
		xm=(verts[2].z-grdnt(verts[2].y, verts[0].y, verts[1].y, verts[0].z, verts[1].z))/(verts[2].x-grdnt(verts[2].y, verts[0].y, verts[1].y, verts[0].x, verts[1].x));
	else
		xm=(verts[0].z-grdnt(verts[0].y, verts[1].y, verts[2].y, verts[1].z, verts[2].z))/(verts[0].x-grdnt(verts[0].y, verts[1].y, verts[2].y, verts[1].x, verts[2].x));
		
	if (verts[1].x==verts[2].x)
		ym=(verts[2].z-grdnt(verts[2].x, verts[0].x, verts[1].x, verts[0].z, verts[1].z))/(verts[2].y-grdnt(verts[2].x, verts[0].x, verts[1].x, verts[0].y, verts[1].y));
	else
		ym=(verts[0].z-grdnt(verts[0].x, verts[1].x, verts[2].x, verts[1].z, verts[2].z))/(verts[0].y-grdnt(verts[0].x, verts[1].x, verts[2].x, verts[1].y, verts[2].y));
	
	zo=verts[0].z-verts[0].y*ym-verts[0].x*xm;
	
	yLow=clamp(verts[0].y, 0, cam->dim.y);
	yHgh=clamp(verts[1].y, 0, cam->dim.y);
	
	for (y=(int)yLow+0.5; y<(int)yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		xLow=clamp(grdnt(y, (int)verts[0].y, (int)verts[2-middleOnLeft].y, (int)verts[0].x, (int)verts[2-middleOnLeft].x), 0, cam->dim.x);
		xHgh=clamp(grdnt(y, (int)verts[0].y, (int)verts[1+middleOnLeft].y, (int)verts[0].x, (int)verts[1+middleOnLeft].x), 0, cam->dim.x);
		
		//xLow=clamp(grdnt(y, verts[0].y, verts[2-middleOnLeft].y, verts[0].x, verts[2-middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, verts[0].y, verts[1+middleOnLeft].y, verts[0].x, verts[1+middleOnLeft].x), 0, cam->dim.x);
		
		yo=(int)y*cam->dim.x;
		
		for (x=(int)xLow+0.5; x<(int)xHgh; ++x)
		{
			z=(x+0.5)*xm+(y+0.5)*ym+zo;
			
			if (z<zBuffer[(int)x+yo])
			{
				renderImage->bits[(int)x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[(int)x+yo]=z;
			}
		}
	}
	
	yLow=clamp(verts[1].y, 0, cam->dim.y);
	yHgh=clamp(verts[2].y, 0, cam->dim.y);
	
	for (y=(int)yLow+0.5; y<(int)yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		xLow=clamp(grdnt(y, (int)verts[2].y, (int)verts[0+middleOnLeft].y, (int)verts[2].x, (int)verts[0+middleOnLeft].x), 0, cam->dim.x);
		xHgh=clamp(grdnt(y, (int)verts[2].y, (int)verts[1-middleOnLeft].y, (int)verts[2].x, (int)verts[1-middleOnLeft].x), 0, cam->dim.x);
		
		//xLow=clamp(grdnt(y, verts[2].y, verts[0+middleOnLeft].y, verts[2].x, verts[0+middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, verts[2].y, verts[1-middleOnLeft].y, verts[2].x, verts[1-middleOnLeft].x), 0, cam->dim.x);
		
		yo=(int)y*cam->dim.x;
		
		for (x=(int)xLow+0.5; x<(int)xHgh; ++x)
		{
			z=(x+0.5)*xm+(y+0.5)*ym+zo;
			
			if (z<zBuffer[(int)x+yo])
			{
				renderImage->bits[(int)x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[(int)x+yo]=z;
			}
		}
	}
}
*/

/*

void Scene3D::renderTri(Vctr3<double> vert0, Vctr3<double> vert1, Vctr3<double> vert2, RGBpix color)
{
	Vctr3<double> vertList[3];
	XYint verts[3];
	double vertsZ[3];
	int x, y, i;
	int xLow, xHgh;
	int yLow, yHgh;
	bool middleOnLeft;
	int yo; //the y offset in the pixmap
	double xm, ym, zo, z; //x and y slope and z-offset
	//const double zFctr=-0.5;
	
	//set the array to the verts sorted from bottom to top
	if (vert0.y<vert1.y)
	{
		if (vert0.y<vert2.y)
		{
			vertList[0]=vert0;
			
			if (vert1.y<vert2.y)
			{
				vertList[1]=vert1;
				vertList[2]=vert2;
			}
			else
			{
				vertList[1]=vert2;
				vertList[2]=vert1;
			}
		}
		else
		{
			vertList[0]=vert2;
			vertList[1]=vert0;
			vertList[2]=vert1;
		}
	}
	else
	{
		if (vert1.y<vert2.y)
		{
			vertList[0]=vert1;
			
			if (vert0.y<vert2.y)
			{
				vertList[1]=vert0;
				vertList[2]=vert2;
			}
			else
			{
				vertList[1]=vert2;
				vertList[2]=vert0;
			}
		}
		else
		{
			vertList[0]=vert2;
			vertList[1]=vert1;
			vertList[2]=vert0;
		}
	}
	
	for (i=0; i<3; ++i)
	{
		verts[i].x=vertList[i].x;
		verts[i].y=vertList[i].y;
		vertsZ[i]=vertList[i].z;
	}
	
	middleOnLeft=verts[1].x<grdnt(verts[1].y, verts[0].y, verts[2].y, verts[0].x, verts[2].x);
	
	if (verts[1].y==verts[2].y)
		xm=(vertsZ[2]-grdnt(verts[2].y, verts[0].y, verts[1].y, vertsZ[0], vertsZ[1]))/(verts[2].x-grdnt(verts[2].y, verts[0].y, verts[1].y, verts[0].x, verts[1].x));
	else
		xm=(vertsZ[0]-grdnt(verts[0].y, verts[1].y, verts[2].y, vertsZ[1], vertsZ[2]))/(verts[0].x-grdnt(verts[0].y, verts[1].y, verts[2].y, verts[1].x, verts[2].x));
		
	if (verts[1].x==verts[2].x)
		ym=(vertsZ[2]-grdnt(verts[2].x, verts[0].x, verts[1].x, vertsZ[0], vertsZ[1]))/(verts[2].y-grdnt(verts[2].x, verts[0].x, verts[1].x, verts[0].y, verts[1].y));
	else
		ym=(vertsZ[0]-grdnt(verts[0].x, verts[1].x, verts[2].x, vertsZ[1], vertsZ[2]))/(verts[0].y-grdnt(verts[0].x, verts[1].x, verts[2].x, verts[1].y, verts[2].y));
	
	zo=vertsZ[0]-verts[0].y*ym-verts[0].x*xm;
	
	yLow=clamp(verts[0].y, 0, cam->dim.y);
	yHgh=clamp(verts[1].y, 0, cam->dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		//xLow=clamp(grdnt(y, (int)verts[0].y, (int)verts[2-middleOnLeft].y, (int)verts[0].x, (int)verts[2-middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, (int)verts[0].y, (int)verts[1+middleOnLeft].y, (int)verts[0].x, (int)verts[1+middleOnLeft].x), 0, cam->dim.x);
		
		xLow=clamp(grdnt(y, verts[0].y, verts[2-middleOnLeft].y, verts[0].x, verts[2-middleOnLeft].x), 0, cam->dim.x);
		xHgh=clamp(grdnt(y, verts[0].y, verts[1+middleOnLeft].y, verts[0].x, verts[1+middleOnLeft].x), 0, cam->dim.x);
		
		yo=y*cam->dim.x;
		
		for (x=xLow; x<xHgh; ++x)
		{
			z=x*xm+y*ym+zo;
			
			if (z<zBuffer[x+yo])
			{
				renderImage->bits[x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[x+yo]=z;
			}
		}
	}
	
	yLow=clamp(verts[1].y, 0, cam->dim.y);
	yHgh=clamp(verts[2].y, 0, cam->dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		//all this int casting stops annoying horizontal lines. don't ask me why.
		//xLow=clamp(grdnt(y, (int)verts[2].y, (int)verts[0+middleOnLeft].y, (int)verts[2].x, (int)verts[0+middleOnLeft].x), 0, cam->dim.x);
		//xHgh=clamp(grdnt(y, (int)verts[2].y, (int)verts[1-middleOnLeft].y, (int)verts[2].x, (int)verts[1-middleOnLeft].x), 0, cam->dim.x);
		
		xLow=clamp(grdnt(y, verts[2].y, verts[0+middleOnLeft].y, verts[2].x, verts[0+middleOnLeft].x), 0, cam->dim.x);
		xHgh=clamp(grdnt(y, verts[2].y, verts[1-middleOnLeft].y, verts[2].x, verts[1-middleOnLeft].x), 0, cam->dim.x);
		
		yo=y*cam->dim.x;
		
		for (x=xLow; x<xHgh; ++x)
		{
			z=x*xm+y*ym+zo;
			
			if (z<zBuffer[x+yo])
			{
				renderImage->bits[x+yo]=color;
				//renderImage->bits[x+yo]=clr(clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255), clamp(z*zFctr+128, 0, 255));
				zBuffer[x+yo]=z;
			}
		}
	}
}
*/

Scene3D::Cam::Cam()
{
	rot=Vctr3<double>(); //rotation of cam
	pos=Vctr3<double>(); //position of cam
	fov=60; //field of view
	prsp=1; //if to render perspective (ortho otherwise)
	dim=mkXY(0, 0); //the dimensions to render
}

Scene3D::Cam::~Cam()
{
	
}
