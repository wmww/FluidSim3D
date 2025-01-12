#include "WidapImage.h"

WidapImage::Glyph WidapImage::widapFont[];

bool WidapImage::init=1;

void WidapImage::setupWidapFont()
{
	int i, j;
	
	const double Y[9]={0.0, 0.25, 0.375, 0.625, 0.75, 1.0, 0.5, -0.2, -0.4};
	const double X[5]={0.0, 0.5, 1.0, 0.25, 0.75}; //normal x and y positions
	
	for (int i=0; i<256; ++i)
	{ //set all glyphs to skull and cross bones (unknown glyph)
		widapFont[i].lineNum=13;
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=0.6;
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=0.8;
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=0.8;
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=0.6;
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=0.2;
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=0.2;
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=0.7;
		widapFont[i].lines[1][j].y=Y[2];
	}
	
	
	///special chars
	
	{ ///[space]
		i=32; widapFont[i].lineNum=0; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
	}
	
	{ ///!
		i=33; widapFont[i].lineNum=5; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.1;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.6;
		widapFont[i].lines[1][j].y=0.05;
		++j;
		widapFont[i].lines[0][j].x=0.6;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.05;
	}
	
	{ ///"
		i=34; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[3];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[3];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[4];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[4];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///#
		i=35; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=1;
		++j;
		widapFont[i].lines[0][j].x=0.7;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=0.7;
		widapFont[i].lines[1][j].y=1;
		++j;
		widapFont[i].lines[0][j].x=0;
		widapFont[i].lines[0][j].y=0.3;
		widapFont[i].lines[1][j].x=1;
		widapFont[i].lines[1][j].y=0.3;
		++j;
		widapFont[i].lines[0][j].x=0;
		widapFont[i].lines[0][j].y=0.7;
		widapFont[i].lines[1][j].x=1;
		widapFont[i].lines[1][j].y=0.7;
	}
	
	{ ///%
		i=37; widapFont[i].lineNum=9; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=1;
		widapFont[i].lines[1][j].y=1;
		++j;
		widapFont[i].lines[0][j].x=0.6;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.8;
		widapFont[i].lines[1][j].y=0.2;
		++j;
		widapFont[i].lines[0][j].x=0.8;
		widapFont[i].lines[0][j].y=0.2;
		widapFont[i].lines[1][j].x=1.0;
		widapFont[i].lines[1][j].y=0.1;
		++j;
		widapFont[i].lines[0][j].x=1.0;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.8;
		widapFont[i].lines[1][j].y=0.0;
		++j;
		widapFont[i].lines[0][j].x=0.8;
		widapFont[i].lines[0][j].y=0.0;
		widapFont[i].lines[1][j].x=0.6;
		widapFont[i].lines[1][j].y=0.1;
		++j;
		widapFont[i].lines[0][j].x=0.0;
		widapFont[i].lines[0][j].y=0.9;
		widapFont[i].lines[1][j].x=0.2;
		widapFont[i].lines[1][j].y=1.0;
		++j;
		widapFont[i].lines[0][j].x=0.2;
		widapFont[i].lines[0][j].y=1.0;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.9;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.9;
		widapFont[i].lines[1][j].x=0.2;
		widapFont[i].lines[1][j].y=0.8;
		++j;
		widapFont[i].lines[0][j].x=0.2;
		widapFont[i].lines[0][j].y=0.8;
		widapFont[i].lines[1][j].x=0.0;
		widapFont[i].lines[1][j].y=0.9;
	}
	
	{ ///'
		i=39; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///(
		i=40; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///)
		i=41; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///*
		i=42; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.3;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.7;
		++j;
		widapFont[i].lines[0][j].x=0.1;
		widapFont[i].lines[0][j].y=0.4;
		widapFont[i].lines[1][j].x=0.9;
		widapFont[i].lines[1][j].y=0.6;
		++j;
		widapFont[i].lines[0][j].x=0.9;
		widapFont[i].lines[0][j].y=0.4;
		widapFont[i].lines[1][j].x=0.1;
		widapFont[i].lines[1][j].y=0.6;
	}
	
	{ ///+
		i=43; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.1;
		widapFont[i].lines[0][j].y=0.5;
		widapFont[i].lines[1][j].x=0.9;
		widapFont[i].lines[1][j].y=0.5;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.3;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.7;
	}
	
	{ ///,
		i=44; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=-0.1;
	}
	
	{ ///-
		i=45; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.1;
		widapFont[i].lines[0][j].y=0.5;
		widapFont[i].lines[1][j].x=0.9;
		widapFont[i].lines[1][j].y=0.5;
	}
	
	{ ///.
		i=46; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.1;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.05;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=0.05;
	}
	
	{ /// /
		i=47; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	
	///numbers
	
	{ ///0
		i=48; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///1
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	{ ///2
		i++; widapFont[i].lineNum=6; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///3
		i++; widapFont[i].lineNum=9; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=0.75;
		widapFont[i].lines[1][j].y=0.5;
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=0.5;
		widapFont[i].lines[1][j].x=0.75;
		widapFont[i].lines[1][j].y=0.5;
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=0.75;
		widapFont[i].lines[1][j].y=0.5;
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///4
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[6];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///5
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///6
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///7
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///8
		i++; widapFont[i].lineNum=10; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///9
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	
	///special chars
	
	{ ///:
		i=58; widapFont[i].lineNum=8; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=0.25;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.3;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.3;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.25;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.25;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.2;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.2;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=0.25;
		++j;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=0.65;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.7;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.7;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.65;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.65;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.6;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.6;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=0.65;
	}
	
	{ ///;
		i=59; widapFont[i].lineNum=5; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0.3;
		widapFont[i].lines[0][j].y=0.65;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.7;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.7;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.65;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.65;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.6;
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.6;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=0.65;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.3;
		widapFont[i].lines[1][j].x=0.3;
		widapFont[i].lines[1][j].y=0.1;
	}
	
	{ ///<
		i=60; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	{ ///=
		i=61; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
	}
	
	{ ///>
		i=62; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	
	///Capital letters
	
	{ ///A
		i=65; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=grdnt(Y[2], Y[0], Y[4], X[0], X[1]);
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=grdnt(Y[2], Y[0], Y[4], X[2], X[1]);
		widapFont[i].lines[1][j].y=Y[2];
	}
	
	{ ///B
		i++; widapFont[i].lineNum=9;  delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///C
		i++; widapFont[i].lineNum=5; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	{ ///D
		i++; widapFont[i].lineNum=6; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///E
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=0.75;
		widapFont[i].lines[1][j].y=Y[6];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///F
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=0.75;
		widapFont[i].lines[1][j].y=Y[6];
	}
	
	{ ///G
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[6];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[4];
		widapFont[i].lines[1][j].y=Y[6];
	}
	
	{ ///H
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=0.5;
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=0.5;
	}
	
	{ ///I
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///J
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///K
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///L
		i++; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///M
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
	}
	
	{ ///N
		i++; widapFont[i].lineNum=3; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///O
		i++; widapFont[i].lineNum=6; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
	}
	
	{ ///P
		i++; widapFont[i].lineNum=6; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[2];
	}
	
	{ ///Q
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///R
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///S
		i++; widapFont[i].lineNum=7; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
	}
	
	{ ///T
		i++; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///U
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[1];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///V
		i++; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[0];
	}
	
	{ ///W
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[3];
	}
	
	{ ///X
		i++; widapFont[i].lineNum=2; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///Y
		i++; widapFont[i].lineNum=5; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///Z
		i++; widapFont[i].lineNum=4; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[0];
		++j;
		widapFont[i].lines[0][j].x=X[3];
		widapFont[i].lines[0][j].y=Y[6];
		widapFont[i].lines[1][j].x=X[4];
		widapFont[i].lines[1][j].y=Y[6];
	}
	
	
	///special chars
	
	{ ///?
		i=63; widapFont[i].lineNum=9; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[0];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[5];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[4];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[4];
		widapFont[i].lines[1][j].x=X[2];
		widapFont[i].lines[1][j].y=Y[3];
		++j;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[3];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[2];
		++j;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[2];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[1];
		++j;
		widapFont[i].lines[0][j].x=0.4;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0.1;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0.1;
		widapFont[i].lines[1][j].x=0.6;
		widapFont[i].lines[1][j].y=0.05;
		++j;
		widapFont[i].lines[0][j].x=0.6;
		widapFont[i].lines[0][j].y=0.05;
		widapFont[i].lines[1][j].x=0.5;
		widapFont[i].lines[1][j].y=0;
		++j;
		widapFont[i].lines[0][j].x=0.5;
		widapFont[i].lines[0][j].y=0;
		widapFont[i].lines[1][j].x=0.4;
		widapFont[i].lines[1][j].y=0.05;
	}
	
	{ /// [backslash]
		i=92; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[2];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[0];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
	{ ///_
		i=95; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=0;
		widapFont[i].lines[0][j].y=-0.1;
		widapFont[i].lines[1][j].x=1;
		widapFont[i].lines[1][j].y=-0.1;
	}
	
	for (i=0; i<26; ++i)
	{ //make smalls the same as capitals
		delete[] widapFont[i].lines[0];
		delete[] widapFont[i].lines[1];
		widapFont[i+97].lineNum=widapFont[i+65].lineNum;
		widapFont[i+97].lines[0]=widapFont[i+65].lines[0];
		widapFont[i+97].lines[1]=widapFont[i+65].lines[1];
	}
	
	///special chars
	
	{ ///|
		i=124; widapFont[i].lineNum=1; delete[] widapFont[i].lines[0]; delete[] widapFont[i].lines[1];
		widapFont[i].lines[0]=new XYdbl[widapFont[i].lineNum];
		widapFont[i].lines[1]=new XYdbl[widapFont[i].lineNum];
		j=0;
		widapFont[i].lines[0][j].x=X[1];
		widapFont[i].lines[0][j].y=Y[0];
		widapFont[i].lines[1][j].x=X[1];
		widapFont[i].lines[1][j].y=Y[5];
	}
	
}

void num2bytes(unsigned char *ptr, int byteNum, int num)
{ //converts an int to bytes (used for writing data to bitmap deader
	int i;
	
	for (i=0; i<byteNum; ++i)
	{
		*(ptr+i)=(num/(int)pow(256, i))%256;
	}
}

int bytes2num(unsigned char *ptr, int bytesNum)
{ //converts bytes to an int (used for reading bitmap header)
	int otpt=0;
	int i;
	
	for (i=0; i<bytesNum; ++i)
	{
		otpt+=*(ptr+i)*pow(256, i);
	}
	
	return otpt;
}

WidapImage::WidapImage(int newWdth, int newHght, RGBpix* bitsPtr)
{ //constructor; if bitsPtr is 0 it will make its own bits
	
	if (init) //only run once no matter how many images are made
	{
		setupWidapFont();
		init=0;
	}
	
	dim.x=newWdth;
	dim.y=newHght;
	
	if (bitsPtr)
	{
		bits=bitsPtr;
		deleteBits=0;
	}
	else
	{
		bits=0;
		deleteBits=1;
		newImage(mkXY(newWdth, newHght));
	}
}

WidapImage::~WidapImage()
{
	if (deleteBits && bits)
		delete[] bits;
}

RGBpix* WidapImage::pix(XYint loc)
{ //returns a pointer to the specified pixel
	if (loc.x>=dim.x || loc.y>=dim.y)
	{
		std::cout << std::endl << ">> tried to access outside of image <<" << std::endl;
		
		if (dim.x && dim.y)
			return (bits+clamp((int)loc.x, 0, dim.x-1)+clamp((int)loc.y, 0, dim.y-1)*dim.x);
		else return 0;
	}
	else
	{
		return (bits+loc.x+loc.y*dim.x);
	}
}

void WidapImage::clrFill(RGBpix clr, double alpha)
{ //fills the entire image with a single color
	
	int x, y;
	
	if (alpha>=1) {
		for (x=0; x<dim.x; ++x) {
			for (y=0; y<dim.y; ++y) {
				
				bits[x+y*dim.x]=clr;
			}
		}
	} else if (alpha>=0) {
		
		for (x=0; x<dim.x; ++x) {
			for (y=0; y<dim.y; ++y) {
				
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	}
}

void WidapImage::rect(int left, int right, int bottom, int top, RGBpix clr, double alpha)
{ //draws a filled rectangle
	int x, y;
	
	left=clamp(left, 0, dim.x);
	right=clamp(right, 0, dim.x);
	top=clamp(top, 0, dim.y);
	bottom=clamp(bottom, 0, dim.y);
	
	if (alpha>=1) {
		for (x=left; x<right; ++x) {
			for (y=bottom; y<top; ++y) {
				
				bits[x+y*dim.x]=clr;
			}
		}
	} else if (alpha>=0) {
		
		for (x=left; x<right; ++x) {
			for (y=bottom; y<top; ++y) {
				
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	}
}

/*void WidapImage::circle(XYint pos, double radius, RGBpix clr, double alpha)
{ //draws a filled circle
	
	int x, y;
	
	double top, btm;
	
	double left, right;
	
	//radius-=0.1; //makes circles look less polygony
	
	//if (alpha>=1) {
		
		left=clamp(pos.x-radius, 0, dim.x);
		right=clamp(pos.x+radius, 0, dim.x);
		
		for (x=left+0.5; x<right; ++x)
		{	
			btm=clamp(pos.y-sqrt(radius*radius-(x-pos.x)*(x-pos.x))+0.75, 0, dim.y);
			top=clamp(pos.y+sqrt(radius*radius-(x-pos.x)*(x-pos.x))-0.25, 0, dim.y);
			
			for (y=btm+0.5; y<top; ++y) {
				
				//bits[x+y*dim.x]=clr;
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	*//*} else if (alpha>=0) {
		
		left=clamp(xLoc-radius, 0, dim.x);
		right=clamp(xLoc+radius, 0, dim.x);
		
		for (x=left; x<right; ++x) {
			
			btm=clamp(yLoc-sqrt(radius*radius-(x-xLoc)*(x-xLoc)), 0, dim.y);
			top=clamp(yLoc+sqrt(radius*radius-(x-xLoc)*(x-xLoc)), 0, dim.y);
			
			for (y=btm; y<top; ++y) {
				
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	}*//*
}
*/

void WidapImage::circle(XYint pos, double radius, RGBpix clr, double alpha)
{ //draws a filled circle
	
	///WARNING: this function is vital for drawn lines to look right. Do not fuck it up without testing it with line()
	
	int x, y;
	
	double top, btm;
	
	double left, right;
	
	//radius-=0.1; //makes circles look less polygony
	
	//if (alpha>=1) {
		
		left=clamp(pos.x-radius, 0, dim.x);
		right=clamp(pos.x+radius, 0, dim.x);
		
		for (x=left; x<right; ++x)
		{	
			btm=clamp(pos.y-sqrt(radius*radius-(x-pos.x)*(x-pos.x))+1, 0, dim.y);
			top=clamp(pos.y+sqrt(radius*radius-(x-pos.x)*(x-pos.x)), 0, dim.y);
			
			for (y=btm; y<top; ++y) {
				
				//bits[x+y*dim.x]=clr;
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	/*} else if (alpha>=0) {
		
		left=clamp(xLoc-radius, 0, dim.x);
		right=clamp(xLoc+radius, 0, dim.x);
		
		for (x=left; x<right; ++x) {
			
			btm=clamp(yLoc-sqrt(radius*radius-(x-xLoc)*(x-xLoc)), 0, dim.y);
			top=clamp(yLoc+sqrt(radius*radius-(x-xLoc)*(x-xLoc)), 0, dim.y);
			
			for (y=btm; y<top; ++y) {
				
				blend(bits+x+y*dim.x, clr, alpha);
			}
		}
	}*/
}

void WidapImage::triangle(XYint vert0, XYint vert1, XYint vert2, RGBpix clr, double alpha)
{ //draws a filled triangle
	
	XYint verts[3];
	int x, y;
	int xLow, xHgh;
	int yLow, yHgh;
	bool middleOnLeft;
	int yo; //the y offset in the pixmap
	
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
	
	yLow=clamp(verts[0].y, 0, dim.y);
	yHgh=clamp(verts[1].y, 0, dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		xLow=clamp(grdnt(y, verts[0].y, verts[2-middleOnLeft].y, verts[0].x, verts[2-middleOnLeft].x), 0, dim.x);
		xHgh=clamp(grdnt(y, verts[0].y, verts[1+middleOnLeft].y, verts[0].x, verts[1+middleOnLeft].x), 0, dim.x);
		
		yo=y*dim.x;
		
		if (alpha>=1)
		{
			for (x=xLow; x<xHgh; ++x)
			{
				bits[x+yo]=clr;
			}
		}
		else if (alpha>=0)
		{
			for (x=xLow; x<xHgh; ++x)
			{
				blend(bits+x+yo, clr, alpha);
			}
		}
	}
	
	yLow=clamp(verts[1].y, 0, dim.y);
	yHgh=clamp(verts[2].y, 0, dim.y);
	
	for (y=yLow; y<yHgh; ++y)
	{
		xLow=clamp(grdnt(y, verts[2].y, verts[0+middleOnLeft].y, verts[2].x, verts[0+middleOnLeft].x), 0, dim.x);
		xHgh=clamp(grdnt(y, verts[2].y, verts[1-middleOnLeft].y, verts[2].x, verts[1-middleOnLeft].x), 0, dim.x);
		
		yo=y*dim.x;
		
		if (alpha>=1)
		{
			for (x=xLow; x<xHgh; ++x)
			{
				bits[x+yo]=clr;
			}
		}
		else if (alpha>=0)
		{
			for (x=xLow; x<xHgh; ++x)
			{
				blend(bits+x+yo, clr, alpha);
			}
		}
	}
}

void WidapImage::target(int left, int right, int bottom, int top, int thickness, RGBpix clr, double alpha)
{ //draws the corners of a square
	
	int lngth=(right-left+top-bottom)/12;
	
	//upper left:
	rect(left-thickness, left, top-lngth, top, clr, alpha);
	rect(left-thickness, left+lngth, top, top+thickness, clr, alpha);
	
	//upper right:
	rect(right-lngth, right+thickness, top, top+thickness, clr, alpha);
	rect(right, right+thickness, top-lngth, top, clr, alpha);
	
	//lower right:
	rect(right, right+thickness, bottom, bottom+lngth, clr, alpha);
	rect(right-lngth, right+thickness, bottom-thickness, bottom, clr, alpha);
	
	//lower left:
	rect(left-thickness, left, bottom, bottom+lngth, clr, alpha);
	rect(left-thickness, left+lngth, bottom-thickness, bottom, clr, alpha);
}

/*void WidapImage::line(LineData data, double thick, RGBpix clr, double alpha)
{
	double x, y;
	double vx, vy; //vector
	double l, r, b, t;
	double lx, ly, rx, ry, bx, by, tx, ty; 
	
	vx=fabs((data.b.x-data.a.x)*thick*0.5/dst(data.b.x-data.a.x, data.b.y-data.a.y));
	vy=fabs((data.b.y-data.a.y)*thick*0.5/dst(data.b.x-data.a.x, data.b.y-data.a.y));
	
	if (data.a.x==data.b.x && data.a.y==data.b.y)
		return;
	
	if (data.b.x>data.a.x)
	{
		lx=data.a.x;
		ly=data.a.y;
		rx=data.b.x;
		ry=data.b.y;
	}
	else
	{
		lx=data.b.x;
		ly=data.b.y;
		rx=data.a.x;
		ry=data.a.y;
	}
	
	if (data.b.y>data.a.y)
	{
		by=data.a.y;
		bx=data.a.x;
		ty=data.b.y;
		tx=data.b.x;
	}
	else
	{
		by=data.b.y;
		bx=data.b.x;
		ty=data.a.y;
		tx=data.a.x;
	}
	
	if ((data.b.x>data.a.x)==(data.b.y>data.a.y))
	{
		ly+=vx;
		ry-=vx;
		bx+=vy;
		tx-=vy;
	}
	else
	{
		ly-=vx;
		ry+=vx;
		bx-=vy;
		tx+=vy;
	}
	
	lx-=vy;
	rx+=vy;
	by-=vx;
	ty+=vx; 
	
	l=clamp(lx, 0, dim.x);
	r=clamp(rx, 0, dim.x);
	
	for (x=l+0.5; x<r; ++x)
	{
		if ((int)x<tx)
			t=grdnt((int)x, lx, tx, ly, ty);
		else
			t=grdnt((int)x, tx, rx, ty, ry);
		
		if ((int)x<bx)
			b=grdnt((int)x, lx, bx, ly, by);
		else
			b=grdnt((int)x, bx, rx, by, ry);
		
		b=clamp(b, 0, dim.y);
		t=clamp(t, 0, dim.y);
		
		for (y=b+0.5; y<t; ++y)
		{
			bits[int(x)+(int(y))*dim.x]=clr;
		}
	}
	
	*//*for (x=l+0.5; x<r; ++x)
	{
		if (x<tx)
			t=grdnt(x, lx, tx, ly, ty);
		else
			t=grdnt(x, tx, rx, ty, ry);
		
		if (x<bx)
			b=grdnt(x, lx, bx, ly, by);
		else
			b=grdnt(x, bx, rx, by, ry);
		
		b=clamp(b, 0, dim.y-0.5);
		t=clamp(t, 0, dim.y-0.5);
		
		for (y=b; y<t; ++y)
		{
			bits[int(x+0.5)+(int(y+0.5))*dim.x]=clr;
		}
	}*//*
	
	//circle(data.a.x, data.a.y, thick/2, clr, alpha);
	//circle(data.b.x, data.b.y, thick/2, clr, alpha);
}*/

/*void WidapImage::line(LineData data, double thick, RGBpix clr, double alpha)
{
	int x, y, i;
	int *big, *sml;
	int bigStrt, bigEnd, smlStrt, smlEnd, smlOffset;
	
	if (fabs(data.a.y-data.b.y)>fabs(data.a.x-data.b.x))
	{
		big=&y;
		sml=&x;
		if (data.a.y<data.b.y)
		{
			bigStrt=data.a.y;
			bigEnd=data.b.y;
			smlStrt=data.a.x;
			smlEnd=data.b.x;
		}
		else
		{
			bigStrt=data.b.y;
			bigEnd=data.a.y;
			smlStrt=data.b.x;
			smlEnd=data.a.x;
		}
	}
	else
	{
		big=&x;
		sml=&y;
		if (data.a.x<data.b.x)
		{
			bigStrt=data.a.x;
			bigEnd=data.b.x;
			smlStrt=data.a.y;
			smlEnd=data.b.y;
		}
		else
		{
			bigStrt=data.b.x;
			bigEnd=data.a.x;
			smlStrt=data.b.y;
			smlEnd=data.a.y;
		}
	}
	
	if (bigStrt!=bigEnd)
	{
		smlOffset=(thick-thick/2)-thick; //this line is odd so int rounding will work right (nothing is -)
		for (i=smlOffset; i<thick/2.0; ++i)
		{
			for (*big=bigStrt; *big<=bigEnd; ++*big)
			{
				//((inpt-strtI)*(strtO-endO))/(strtI-endI)+strtO;
				*//*sml=((*big-bigStrt)*(smlStrt-smlEnd))/(bigStrt-bigEnd)+smlStrt;
				*sml=grdnt(*big, bigStrt, bigEnd, smlStrt, smlEnd)+0.5+i;
				
				if (x>0 && x<dim.x && y>0 && y<dim.y)
					bits[x+y*dim.x]=clr;
			}
		}
	}
	
	//circle(data.a, thick/2.0, clr, alpha);
	//circle(data.b, thick/2.0, clr, alpha);
}
*/

/*void WidapImage::line(XYint a, XYint b, int thick, RGBpix clr, double alpha)
{
	int x, y, i;
	int xShft, yShft, shftSum;;
	int xOfst, yOfst;
	int *big, *sml;
	int bigStrt, bigEnd, smlStrt, smlEnd;
	
	xShft=floor(thick*(a.y-b.y)/dst(a, b)+0.5+thick)-thick;
	yShft=-floor(thick*(a.x-b.x)/dst(a, b)+0.5+thick)+thick;
	shftSum=abs(xShft)+abs(yShft);
	
	if (thick<2)
		shftSum=1;
	
	//std::cout << "xShift: " << xShft << ", yShft: " << yShft << "\n";
	
	//printf(txt, "xShft: %d, yShft: %d", xShft, yShft);
	
	if (fabs(a.y-b.y)>fabs(a.x-b.x))
	{
		big=&y;
		sml=&x;
		if (a.y<b.y)
		{
			bigStrt=a.y;
			bigEnd=b.y;
			smlStrt=a.x;
			smlEnd=b.x;
		}
		else
		{
			bigStrt=b.y;
			bigEnd=a.y;
			smlStrt=b.x;
			smlEnd=a.x;
		}
	}
	else
	{
		big=&x;
		sml=&y;
		if (a.x<b.x)
		{
			bigStrt=a.x;
			bigEnd=b.x;
			smlStrt=a.y;
			smlEnd=b.y;
		}
		else
		{
			bigStrt=b.x;
			bigEnd=a.x;
			smlStrt=b.y;
			smlEnd=a.y;
		}
	}
	
	if (bigStrt!=bigEnd)
	{
		xOfst=floor(-xShft/2.0+0.5);
		yOfst=floor(-yShft/2.0+0.5);
		for (i=0; i<shftSum; ++i)
		{
			for (*big=bigStrt; *big<=bigEnd; ++*big)
			{
				//((inpt-strtI)*(strtO-endO))/(strtI-endI)+strtO;
				*sml=((*big-bigStrt)*(smlStrt-smlEnd)+(bigStrt-bigEnd)*0.5)/(bigStrt-bigEnd)+smlStrt;
				// *sml=grdnt(*big, bigStrt, bigEnd, smlStrt, smlEnd)+0.5+i;
				
				if (x+xOfst>=0 && x+xOfst<dim.x && y+yOfst>=0 && y+yOfst<dim.y)
					bits[(x+xOfst)+(y+yOfst)*dim.x]=clr;
			}
			
			if (!yShft)
				xOfst+=2*(xShft>0)-1;
			else if (!xShft)
				yOfst+=2*(yShft>0)-1;
			else if (abs(xOfst-(xShft*i/shftSum-xShft/2))<abs(yOfst-(yShft*i/shftSum-yShft/2)))
				xOfst+=2*(xShft>0)-1;
			else
				yOfst+=2*(yShft>0)-1;
		}
	}
	
	//blend(pix(a.x+xShft, a.y+yShft), ::clr(255, 255, 255), 0.5);
	//blend(pix(a.x-xShft, a.y-yShft), ::clr(255, 255, 255), 0.5);
	
	if (shftSum>1)
	{
		//circle(a, thick/2.0, ::clr(0, 128, 255), alpha);
		//circle(b, thick/2.0, ::clr(0, 128, 255), alpha);
		
		circle(a, thick/2.0, clr, alpha);
		circle(b, thick/2.0, clr, alpha);
	}
}*/

void WidapImage::line(XYint a, XYint b, int thick, RGBpix clr)
{
	int x, y, i;
	int xShft, yShft, shftSum;;
	int xOfst, yOfst;
	int *big, *sml;
	int bigStrt, bigEnd, smlStrt, smlEnd;
	
	xShft=floor((thick-1)*(a.y-b.y)/dst(a, b)+0.5);
	yShft=floor(-(thick-1)*(a.x-b.x)/dst(a, b)+0.5);
	shftSum=abs(xShft)+abs(yShft);
	
	//if (thick<2)
		//shftSum=1;
	
	//std::cout << "xShift: " << xShft << ", yShft: " << yShft << "\n";
	
	//printf(txt, "xShft: %d, yShft: %d", xShft, yShft);
	
	if (fabs(a.y-b.y)>fabs(a.x-b.x))
	{
		big=&y;
		sml=&x;
		if (a.y<b.y)
		{
			bigStrt=a.y;
			bigEnd=b.y;
			smlStrt=a.x;
			smlEnd=b.x;
		}
		else
		{
			bigStrt=b.y;
			bigEnd=a.y;
			smlStrt=b.x;
			smlEnd=a.x;
		}
	}
	else
	{
		big=&x;
		sml=&y;
		if (a.x<b.x)
		{
			bigStrt=a.x;
			bigEnd=b.x;
			smlStrt=a.y;
			smlEnd=b.y;
		}
		else
		{
			bigStrt=b.x;
			bigEnd=a.x;
			smlStrt=b.y;
			smlEnd=a.y;
		}
	}
	
	if (bigStrt!=bigEnd)
	{
		xOfst=floor(-xShft/2.0+0.5);
		yOfst=floor(-yShft/2.0+0.5);
		
		for (i=0; i<=shftSum; ++i)
		{
			for (*big=bigStrt; *big<=bigEnd; ++*big)
			{
				//((inpt-strtI)*(strtO-endO))/(strtI-endI)+strtO;
				*sml=((*big-bigStrt)*(smlStrt-smlEnd)+(bigStrt-bigEnd)*0.5)/(bigStrt-bigEnd)+smlStrt;
				//*sml=grdnt(*big, bigStrt, bigEnd, smlStrt, smlEnd)+0.5+i;
				
				if (x+xOfst>=0 && x+xOfst<dim.x && y+yOfst>=0 && y+yOfst<dim.y)
					bits[(x+xOfst)+(y+yOfst)*dim.x]=clr;
			}
			
			if (abs(xOfst-floor(xShft/2.0+0.5))>abs(yOfst-floor(yShft/2.0+0.5)))
			{
				if (xOfst<floor(xShft/2.0+0.5))
					xOfst++;
				else
					xOfst--;
			}
			else
			{
				if (yOfst<floor(yShft/2.0+0.5))
					yOfst++;
				else
					yOfst--;
			}
		}
	}
	
	//blend(pix(a.x+xShft, a.y+yShft), ::clr(255, 255, 255), 0.5);
	//blend(pix(a.x-xShft, a.y-yShft), ::clr(255, 255, 255), 0.5);
	
	//if (shftSum>1)
	{
		//circle(a, thick/2.0-0.25, ::clr(0, 128, 255), 0.5);
		//circle(b, thick/2.0-0.25, ::clr(0, 128, 255), 0.5);
		
		circle(a, thick/2.0-0.5, clr);
		circle(b, thick/2.0-0.5, clr);
	}
}

void WidapImage::text(char txt, double pixHgh, RGBpix clr, double weight)
{
	int j;
	int lineThick, pixWde; //thickness of the lines and number of pixels each char is wide
	
	XYint a, b;
	
	lineThick=weight*pixHgh/16;
	if (lineThick<1)
		lineThick=1;
	pixWde=pixHgh*0.5;
	
	if (txt=='\n')
	{
		loc.y-=pixHgh*1.5;
		loc.x=locStrt.x;
	}
	else if (txt=='\b')
	{
		//loc.x-=pixWde*1.5;
	}
	else if (txt== '	')
	{
		loc.x=(int)((loc.x-locStrt.x)/(pixWde*6)+1)*6*pixWde+locStrt.x;
	}
	else
	{
		for (j=0; j<widapFont[(int)txt].lineNum; ++j)
		{
			a.x=widapFont[(int)txt].lines[0][j].x*pixWde+loc.x;
			a.y=widapFont[(int)txt].lines[0][j].y*pixHgh+loc.y;
			b.x=widapFont[(int)txt].lines[1][j].x*pixWde+loc.x;
			b.y=widapFont[(int)txt].lines[1][j].y*pixHgh+loc.y;
			line(a, b, lineThick, clr);
		}
		
		loc.x+=pixWde*1.5;
	}
}

void WidapImage::text(const char * txt, double pixHgh, RGBpix clr, double weight)
{ //draws text
	
	int i=0;
	
	const int MAX_LNG=2048; //max text length, stops non terminated strings from crashing everything
	
	while (txt[i]!=0 && i<MAX_LNG)
	{
		text(txt[i], pixHgh, clr, weight);
		
		++i;
	}
	if (i==MAX_LNG)
		std::cout << ">> string sent to WidapImage::text() was not terminated before position " << MAX_LNG << "<<\n";
}

void WidapImage::setBits(XYint newDim, RGBpix* bitsPtr)
{ //sets a new pointer for the image (if zero does not actually make an image, you need to call load or newImage for that)
	
	if (deleteBits)
		delete[] bits;
	
	deleteBits=0;
	
	if (newDim.x>=0 && newDim.y>=0)
	{
		dim=newDim;
	}
	
	bits=bitsPtr;
}

void WidapImage::newImage(XYint newDim)
{ //makes a new image of the specified size but does not fill it with any color
	
	if (deleteBits && bits)
		delete[] bits;
	
	deleteBits=1;
	dim=newDim;
	if (dim.x>0 && dim.y>0)
		bits=new RGBpix[dim.x*dim.y];
	else
		bits=0;
}

bool WidapImage::load(const char* filenameInpt)
{ //loads a single bitmap into the image
	
	FILE *fp; //file pointer
	unsigned char header[54];
	int len;
	char filename[64];
	
	strcpy(filename, filenameInpt);
	
	len=strlen(filename);
	
	if (len<4 || filename[len-4]!='.')
		strcat(filename, ".bmp");
	
	fp=fopen(filename, "rb");
	
	if (deleteBits && bits)
	{
		delete[] bits;
		bits=0;
		dim.x=0;
		dim.y=0;
	}
	
	if (fp==NULL) //error
	{
		std::cout << ">> Failed to load image '" << filename << "'! <<" << std::endl;
		return 1;
	}
	else
	{
		if (!fread(header, 1, 54, fp))
			std::cout << ">> file error <<" << std::endl;
		
		dim.x=bytes2num(header+18, 4);
		dim.y=bytes2num(header+22, 4);
		
		if (dim.x!=(dim.x/4)*4 || dim.y!=(dim.y/4)*4)
			std::cout << std::endl << ">> loaded bitmap dimensions not divisible by 4; expect strange results <<" << std::endl;
		
		if (deleteBits)
			bits=new RGBpix[dim.x*dim.y];
		
		if (!fread(bits, 1, dim.x*dim.y*3, fp))
			std::cout << ">> file error <<" << std::endl;
		
		fclose(fp);
		
		return 0;
	}
}

void WidapImage::trnsfr(WidapImage* othrImg, int xLoc, int yLoc)
{ //transfers this image onto another one; x and y specify the lower left corner
	
	if (!bits)
	{
		std::cout << ">> tried to transfer, but image empty <<" << std::endl;
		return;
	}
	
	if (!othrImg->bits)
		othrImg->newImage(dim);
	
	int xMin=clamp(xLoc, 0, othrImg->getWdth());
	int xMax=clamp(xLoc+dim.x, 0, othrImg->getWdth());
	int yMin=clamp(yLoc, 0, othrImg->getHght());
	int yMax=clamp(yLoc+dim.y, 0, othrImg->getHght());
	
	int othrImgWdth=othrImg->getWdth();
	
	int x, y;
	int yFactor, yFactor1;
	
	for (y=yMin; y<yMax; ++y) {
		
		yFactor=y*othrImgWdth;
		yFactor1=-xLoc+(y-yLoc)*dim.x;
		
		for (x=xMin; x<xMax; ++x) {
			othrImg->bits[x+yFactor]=bits[x+yFactor1];
		}
	}
}

void WidapImage::save(const char* filenameInpt)
{ //saves the image to a bitmap
	
	unsigned char header[54];
	char filename[256];
	FILE *fp; //file pointer
	
	if (!bits)
	{
		std::cout << ">> tried to save empty image <<" << std::endl;
		return;
	}
	
	strcpy(filename, filenameInpt);
	
	if (dim.x!=(dim.x/4)*4 || dim.y!=(dim.y/4)*4)
		std::cout << std::endl << ">> saved bitmap dimensions not divisible by 4; expect strange results <<" << std::endl;
	
	//bitmap header info found at http://www.fastgraph.com/help/bmp_header_format.html
	num2bytes(header+0, 2, 19778); //BM
	num2bytes(header+2, 4, dim.x*dim.y*3+54); //size of file
	num2bytes(header+6, 2, 0);
	num2bytes(header+8, 2, 0);
	num2bytes(header+10, 4, 54); //"offset to start of image data in bytes"; not sure what this means
	num2bytes(header+14, 4, 40); //size of remaining header
	num2bytes(header+18, 4, dim.x);
	num2bytes(header+22, 4, dim.y);
	num2bytes(header+26, 2, 1);
	num2bytes(header+28, 2, 24); //bits per pixel
	num2bytes(header+30, 4, 0); //no compression
	num2bytes(header+34, 4, (dim.x/4)*4*(dim.y/4)*4*3); //size of image data in bytes (including padding)
	num2bytes(header+38, 4, 2835); //72 dpi in pixels per meeter
	num2bytes(header+42, 4, 2835); //72 dpi in pixels per meeter
	num2bytes(header+46, 4, 0);
	num2bytes(header+50, 4, 0);
	
	int len=strlen(filename);
	
	if (len<4 || filename[len-4]!='.')
		strcat(filename, ".bmp");
	
	fp=fopen(filename, "wb");
	
	if (fp==NULL) //error
	{
		std::cout << std::endl << ">> Failed to save image '" << filename << "'! <<" << std::endl;
	}
	
	else
	{
		if (!fwrite(header, 1, 54, fp) || !fwrite(bits, 1, dim.x*dim.y*3, fp))
			std::cout << ">> file error <<" << std::endl;
		
		fclose(fp);
	}
}

