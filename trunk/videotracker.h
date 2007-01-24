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
#define PI 3.14159265
//! \struct Coordinate of the tracked object in this case a ball.
struct coordinate { 
	//!Coordinates of the rectangle around the blob
	int Minx,Maxx,Maxy,Miny;
	int cX;
	int cY;
	//!float radius;
	bool flag;
};



struct matrixDesc{
		int nCols;
		int nRows;
};

#endif
