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

//! Including the standard C++ library header
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

//! the numeber of matrix in the data.txt file to parse
#define NUMBER_OF_MATRIX 7

//! \struct Coordinate of the tracked object in this case a ball.
struct coordinate { 
	//!Coordinates of the rectangle around the blob
	int Minx,Maxx,Maxy,Miny;
	//!float radius;
	bool flag;
};

struct matrixDesc{
		int nCols;
		int nRows;
};


//! All the prototype functions of the video-tracker
void initKalman(CvKalman * kalman, CvMat** indexMat);

void execute(char * aviName,int id );

std::vector<float> getValue(wxString filename);

IplImage* getBackground(char* aviName);

struct coordinate extractBlob(IplImage* tmp_frame, IplImage* background,int id);

void parse(wxString fileName,float* ValuesVect,struct matrixDesc* MDSC);

IplImage* getFiltredBinaryImage(IplImage* currentImage, IplImage* backgroundImage, int value );

void drawBlob(IplImage * image, struct coordinate coord,int R, int G, int B );
	
void copyMat (CvMat* source, CvMat* dest);

float* updateKalman(CvKalman * kalman,CvMat *state, CvMat* measurement, CvMat * process_noise, struct coordinate coord);
