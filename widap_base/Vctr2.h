#pragma once
#include "WidapStd.h"
#include <iostream>

template<typename T>
class Vctr2
{
public:
	T x, y;
	
	Vctr2()
	{
		x=T();
		y=T();
	}
	
	Vctr2(const Vctr2<T>& inpt)
	{
		x=inpt.x;
		y=inpt.y;
	}
	
	Vctr2(T newX, T newY)
	{
		x=newX; y=newY;
	}
	
	Vctr2(XYint inpt)
	{
		x=inpt.x;
		y=inpt.y;
	}
	
	Vctr2(XYdbl inpt)
	{
		x=inpt.x;
		y=inpt.y;
	}
	
	T dst()
	{
		return sqrt(x*x+y*y);
	}
	
	T dstSquared()
	{
		return x*x+y*y;
	}
	
	T area()
	{
		return x*y;
	}
	
	template<typename T0>
	operator Vctr2<T0>()
	{
		return Vctr2<T0>(x, y);
	}
	
	operator bool()
	{
		return ((bool)x || (bool)y);
	}
	
	operator XYint()
	{
		XYint otpt;
		otpt.x=x;
		otpt.y=y;
		return otpt;
	}
	
	operator XYdbl()
	{
		XYdbl otpt;
		otpt.x=x;
		otpt.y=y;
		return otpt;
	}
	
	template<typename T0>
	operator T0()
	{
		return this->dst();
	}
	
	template<typename T0>
	void operator= (const Vctr2<T0>& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		//return *this;
	}
	
	void operator= (const XYint& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		//return *this;
	}
	
	void operator= (const XYdbl& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		//return *this;
	}
	
	/*template<typename T0>
	bool operator== (const Vctr2<T0>& inpt)
	{
		return x==inpt.x && y==inpt.y && z==inpt.z;
	}
	
	template<typename T0>
	bool operator!= (const Vctr2<T0>& inpt)
	{
		return x!=inpt.x || y!=inpt.y || z!=inpt.z;
	}
	
	template<typename T0>
	void operator= (const Vctr2<T0>& inpt)
	{
		x=inpt.x;
		y=inpt.y;
		z=inpt.z;
		//return *this;
	}
	
	template<typename T0>
	Vctr2 operator+ (const Vctr2<T0>& inpt)
	{
		return Vctr2(x+inpt.x, y+inpt.y, z+inpt.z);
	}
	
	template<typename T0>
	void operator+= (const Vctr2<T0>& inpt)
	{
		x+=inpt.x;
		y+=inpt.y;
		z+=inpt.z;
		//return Vctr2(x+inpt.x, y+inpt.y, z+inpt.z);
	}
	
	template<typename T0>
	void operator-= (const Vctr2<T0>& inpt)
	{
		x-=inpt.x;
		y-=inpt.y;
		z-=inpt.z;
		//return Vctr2(x-inpt.x, y-inpt.y, z-inpt.z);
	}
	
	template<typename T0>
	Vctr2 operator- (const Vctr2<T0>& inpt)
	{
		return Vctr2(x-inpt.x, y-inpt.y, z-inpt.z);
	}
	
	template<typename T0>
	Vctr2 operator* (const Vctr2<T0>& inpt)
	{
		return Vctr2(x*inpt.x, y*inpt.y, z*inpt.z);
	}
	
	template<typename T0>
	Vctr2 operator* (T0 inpt)
	{
		return Vctr2(x*inpt, y*inpt, z*inpt);
	}
	
	template<typename T0>
	Vctr2 operator/ (const Vctr2<T0>& inpt)
	{
		return Vctr2(x/inpt.x, y/inpt.y, z/inpt.z);
	}
	
	template<typename T0>
	Vctr2 operator/ (T0 inpt)
	{
		return Vctr2(x/inpt, y/inpt, z/inpt);
	}
	
	template<typename T0>
	T dot(const Vctr2<T0>& b)
	{
		//dot product
		return x*b.x+y*b.y+z*b.z;
	}
	
	template<typename T0>
	Vctr2 cross(const Vctr2<T0>& b)
	{
		//cross product
		return Vctr2(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
	}
	
	Vctr2 floor()
	{
		return Vctr2(::floor(x), ::floor(y), ::floor(z));
	}
	
	template<typename T0>
	double ang(const Vctr2<T0>& b)
	{
		//get the angle between two vectors
		return acos((x*b.x+y*b.y+z*b.z)/sqrt((x*x+y*y+z*z)*(b.x*b.x+b.y*b.y+b.z*b.z))); //acos(dot(b)/(dst()*b.dst()))
	}
	
	//returns if the vector is inside the input, as when the input is dimensions and this vector is a location
	//will always return false if input components are not all positive
	//inclusive with 0, exclusive for the input vector
	bool isInside(Vctr2 inpt)
	{
		return (x>=0 && y>=0 && z>=0 && x<inpt.x && y<inpt.y && z<inpt.z) ? 1 : 0;
	}
	
	static Vctr2 min(const Vctr2& a, const Vctr2& b)
	{
		return Vctr2(a.x<b.x?a.x:b.x, a.y<b.y?a.y:b.y, a.z<b.z?a.z:b.z);
	}
	
	static Vctr2 max(const Vctr2& a, const Vctr2& b)
	{
		return Vctr2(a.x>b.x?a.x:b.x, a.y>b.y?a.y:b.y, a.z>b.z?a.z:b.z);
	}
	
	Vctr2 clamp(const Vctr2& b)
	{
		return Vctr2(x<0?0:x>b.x?b.x:x,y<0?0:y>b.y?b.y:y,z<0?0:z>b.z?b.z:z);
	}
	
	static bool touches(Vctr2 a, Vctr2 b, Vctr2 c, Vctr2 d) //if the box made by a and b touches the box made by b and c
	{ //a and c should always be min and b and d should always be max
		return (b.x>=c.x && a.x<=d.x && b.y>=c.y && a.y<=d.y && b.z>=c.z && a.z<=d.z);
	}
	
	static Vctr2 cross(const Vctr2& a, const Vctr2& b)
	{
		return Vctr2(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
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
	
	Vctr2 normalized() //returns this vector with a length of 1, unlike normalize, does not change this vector
	{
		T dvdr=dst();
		return Vctr2(x/dvdr, y/dvdr, z/dvdr);
	}
	
	void inverse() //sets the vector to its inverse, its like normalizing but more so
	{
		T dvdr=dstSquared();
		x/=dvdr;
		y/=dvdr;
		z/=dvdr;
	}
	
	Vctr2 inversed() //returns a vector that is the inversed version of this one
	{
	
	
		T dvdr=dstSquared(); //like the distance formula but without the sqrt()
		return Vctr2(x/dvdr, y/dvdr, z/dvdr);
	}*/
};
