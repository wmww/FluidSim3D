#pragma once
#include "WidapStd.h"
#include <iostream>

template<typename T>
class Vctr3
{
public:
	T x, y, z;
	
	Vctr3()
	{
		x=T();
		y=T();
		z=T();
	}
	
	Vctr3(const Vctr3<T>& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		z=inpt.z;
	}
	
	Vctr3(T newX, T newY, T newZ)
	{
		x=newX; y=newY; z=newZ;
	}
	
	T dst()
	{
		return sqrt(x*x+y*y+z*z);
	}
	
	T dstSquared()
	{
		return x*x+y*y+z*z;
	}
	
	T volume()
	{
		return x*y*z;
	}
	
	template<typename T0>
	operator Vctr3<T0>()
	{
		return Vctr3<T0>(x, y, z);
	}
	
	operator bool()
	{
		return ((bool)x || (bool)y || (bool)z);
	}
	
	template<typename T0>
	operator T0()
	{
		return this->dst();
	}
	
	template<typename T0>
	bool operator== (const Vctr3<T0>& inpt)
	{
		return x==inpt.x && y==inpt.y && z==inpt.z;
	}
	
	template<typename T0>
	bool operator!= (const Vctr3<T0>& inpt)
	{
		return x!=inpt.x || y!=inpt.y || z!=inpt.z;
	}
	
	template<typename T0>
	void operator= (const Vctr3<T0>& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		z=inpt.z;
		//return *this;
	}
	
	template<typename T0>
	Vctr3 operator+ (const Vctr3<T0>& inpt)
	{
		return Vctr3(x+inpt.x, y+inpt.y, z+inpt.z);
	}
	
	template<typename T0>
	void operator+= (const Vctr3<T0>& inpt)
	{
		x+=inpt.x;
		y+=inpt.y;
		z+=inpt.z;
		//return Vctr3(x+inpt.x, y+inpt.y, z+inpt.z);
	}
	
	template<typename T0>
	void operator-= (const Vctr3<T0>& inpt)
	{
		x-=inpt.x;
		y-=inpt.y;
		z-=inpt.z;
		//return Vctr3(x-inpt.x, y-inpt.y, z-inpt.z);
	}
	
	template<typename T0>
	Vctr3 operator- (const Vctr3<T0>& inpt)
	{
		return Vctr3(x-inpt.x, y-inpt.y, z-inpt.z);
	}
	
	template<typename T0>
	Vctr3 operator* (const Vctr3<T0>& inpt)
	{
		return Vctr3(x*inpt.x, y*inpt.y, z*inpt.z);
	}
	
	template<typename T0>
	Vctr3 operator* (T0 inpt)
	{
		return Vctr3(x*inpt, y*inpt, z*inpt);
	}
	
	template<typename T0>
	Vctr3 operator/ (const Vctr3<T0>& inpt)
	{
		return Vctr3(x/inpt.x, y/inpt.y, z/inpt.z);
	}
	
	template<typename T0>
	Vctr3 operator/ (T0 inpt)
	{
		return Vctr3(x/inpt, y/inpt, z/inpt);
	}
	
	template<typename T0>
	T dot(const Vctr3<T0>& b)
	{
		//dot product
		return x*b.x+y*b.y+z*b.z;
	}
	
	template<typename T0>
	Vctr3 cross(const Vctr3<T0>& b)
	{
		//cross product
		return Vctr3(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
	}
	
	Vctr3 floor()
	{
		return Vctr3(::floor(x), ::floor(y), ::floor(z));
	}
	
	template<typename T0>
	double ang(const Vctr3<T0>& b)
	{
		//get the angle between two vectors
		return acos((x*b.x+y*b.y+z*b.z)/sqrt((x*x+y*y+z*z)*(b.x*b.x+b.y*b.y+b.z*b.z))); //acos(dot(b)/(dst()*b.dst()))
	}
	
	//returns if the vector is inside the input, as when the input is dimensions and this vector is a location
	//will always return false if input components are not all positive
	//inclusive with 0, exclusive for the input vector
	bool isInside(Vctr3 inpt)
	{
		return (x>=0 && y>=0 && z>=0 && x<inpt.x && y<inpt.y && z<inpt.z) ? 1 : 0;
	}
	
	static Vctr3 min(const Vctr3& a, const Vctr3& b)
	{
		return Vctr3(a.x<b.x?a.x:b.x, a.y<b.y?a.y:b.y, a.z<b.z?a.z:b.z);
	}
	
	static Vctr3 max(const Vctr3& a, const Vctr3& b)
	{
		return Vctr3(a.x>b.x?a.x:b.x, a.y>b.y?a.y:b.y, a.z>b.z?a.z:b.z);
	}
	
	Vctr3 clamp(const Vctr3& b)
	{
		return Vctr3(x<0?0:x>b.x?b.x:x,y<0?0:y>b.y?b.y:y,z<0?0:z>b.z?b.z:z);
	}
	
	static bool touches(Vctr3 a, Vctr3 b, Vctr3 c, Vctr3 d) //if the box made by a and b touches the box made by b and c
	{ //a and c should always be min and b and d should always be max
		return (b.x>=c.x && a.x<=d.x && b.y>=c.y && a.y<=d.y && b.z>=c.z && a.z<=d.z);
	}
	
	static Vctr3 cross(const Vctr3& a, const Vctr3& b)
	{
		return Vctr3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
	}
	
	//sums the components of the vector
	T sum()
	{
		return x+y+z;
	}
	
	void normalize() //makes the vector have a length of 1
	{
		T dvdr=dst();
		x/=dvdr;
		y/=dvdr;
		z/=dvdr;
	}
	
	Vctr3 normalized() //returns this vector with a length of 1, unlike normalize, does not change this vector
	{
		T dvdr=dst();
		return Vctr3(x/dvdr, y/dvdr, z/dvdr);
	}
	
	void inverse() //sets the vector to its inverse, its like normalizing but more so
	{
		T dvdr=dstSquared();
		x/=dvdr;
		y/=dvdr;
		z/=dvdr;
	}
	
	Vctr3 inversed() //returns a vector that is the inversed version of this one
	{
		T dvdr=dstSquared(); //like the distance formula but without the sqrt()
		return Vctr3(x/dvdr, y/dvdr, z/dvdr);
	}
};

template<typename T>
T posOnLine(Vctr3<T> point, Vctr3<T> lineStrt, Vctr3<T> lineEnd) //find the position on the line (0-1 if between start and end) closest to the point
{
	//I wrote this function based on this:
	//	http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	
	//there is a chance of divide by 0 but since I usually use floating point numbers with this Ill just let it slide
	return -((lineStrt-point).dot(lineEnd-lineStrt))/(lineEnd-lineStrt).dstSquared();
}

template<typename T>
T dstToLine(Vctr3<T> pointOffset, Vctr3<T> lineOffset) //find the position on the line (0-1 if between start and end) closest to the point
{ //				point-lineStrt			lineEnd-lineStrt
	//I wrote this function based on this:
	//	http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	
	if (!lineOffset)
		return pointOffset.dst();
	else
		return (pointOffset)-(lineOffset)*(((pointOffset).dot(lineOffset))/(lineOffset).dstSquared());
}

template<typename T>
T dstToLine(Vctr3<T> point, Vctr3<T> lineStrt, Vctr3<T> lineEnd) //find the position on the line (0-1 if between start and end) closest to the point
{
	//I wrote this function based on this:
	//	http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	
	//return (point-lineStrt)-(lineEnd-lineStrt)*(((point-lineStrt).dot(lineEnd-lineStrt))/(lineEnd-lineStrt).dstSquared());
	return dstToLine(point-lineStrt, lineEnd-lineStrt);
}

template<typename T>
XYdbl mapTriCords(Vctr3<T> pt, Vctr3<T> tri[3])
{ //maps point on a triangle to a location between (0, 0) and (1, 1) (if it is on the triangle
	
	XYdbl cords;
	Vctr3<T> lineOffset=tri[1]-tri[0];
	double posOnBase=posOnLine(tri[2], tri[0], tri[1]); //the position on the base line (vert 0 to vert 1) closest to vert 2
	
	//cords.y=dstToLine(pt-tri[0], lineOffset)/(tri[2]-tri[0]-lineOffset*posOnBase).dst();
	
	cords.y=posOnLine(pt, tri[0]+lineOffset*posOnBase, tri[2]);
	
	if (cords.y==1) //stop divide by zeros
	{
		if (posOnBase==posOnLine(pt, tri[0], tri[1]))
			cords.x=0;
		else
			cords.x=NAN;
	}
	else
		cords.x=(cords.y*posOnBase-posOnLine(pt, tri[0], tri[1]))/(cords.y-1);
	
	//this is the same calculation but more readable and possibly slower
	//cords.x=grdnt(posOnLine(pt, tri[0], tri[1]), cords.y*posOnBase, 1-cords.y*(1-posOnBase), 0, 1);
	
	return cords;
}

