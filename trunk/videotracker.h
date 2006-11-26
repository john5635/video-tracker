#ifndef  _VIDEOTRACKER_H
#define  _VIDEOTRACKER_H


//! Including the wx library header
#include <wx/wx.h> 
#include <wx/string.h>
#include <wx/ffile.h>

//!Including the Opencv library header
#include <cvaux.h>  
#include <highgui.h>
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <cxcore.h>

//! Including the standard C++ library header
#include <vector>
#include <math.h>
#include <iostream>

//! Including Blob header
#include "Blob.h"
#include "BlobResult.h"
using namespace std;

//! the numeber of matrix in the data.txt file to parse
#define NUMBER_OF_MATRIX 6

//! \struct Coordinate of the tracked object in this case a ball.
struct coordinate { 
	//!Coordinates of the rectangle around the blob
	int Minx,Maxx,Maxy,Miny;
	int cX;
	int cY;
	//!float radius;
	bool flag;
};

class coord{
public:
	int MinX;
	int MaxX;
	int MinY;
	int MaxY;
	double cX;
	double cY;
	int lX;
	int lY;
	bool flag;

	coord(){
		MinX=0;
		MaxX=0;
		MinY=0;
		MaxY=0;
		cX=0;
		cY=0;
		lX=0;
		lY=0;
		flag=true;
	}

	coord(int x,int y){
		MinX=0;
		MaxX=0;
		MinY=0;
		MaxY=0;
		cX=x;
		cY=y;
		lX=0;
		lY=0;
		flag=true;
	}

	coord(int maxx,int minx, int maxy, int miny){
		MinX=minx;
		MaxX=maxy;
		MinY=miny;
		MaxY=maxy;
		cX=(MinX+MaxX)/2;
		cY=(MinY+MaxY)/2;
		lX=MaxX-MinX;
		lY=MaxY-MinY;
		flag=true;
	}

	void set(int x,int y){
		cX=x;
		cY=y;
	}

	void set(int maxx,int minx, int maxy, int miny){
		MinX=minx;
		MaxX=maxx;
		MinY=miny;
		MaxY=maxy;
		cX=(MinX+MaxX)/2;
		cY=(MinY+MaxY)/2;
		lX=MaxX-MinX;
		lY=MaxY-MinY;
	}

	//~coord();//distruttore
};

struct matrixDesc{
		int nCols;
		int nRows;
};

#endif

