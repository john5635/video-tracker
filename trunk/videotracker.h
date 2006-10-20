#include <wx/wx.h> 
#include <wx/string.h>

#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <cxcore.h>
#include <vector>
#include <wx/ffile.h>
#include <iostream>

using namespace std;

struct coordinate { // coordinate of the tracked object in this case a ball.
	//Coordinates of the rectangle around the blob
	int Minx,Maxx,Maxy,Miny;
	//float radius;
	bool flag;
};

struct matrixDesc{
		int nCols;
		int nRows;
};

void init(CvKalman * kalman, CvMat** indexMat);
//void execute(CvKalman* kalman, char * aviName );

void execute(char * aviName,int id );

std::vector<float> getValue(wxString filename);


IplImage* getBackground(char* aviName);

struct coordinate extractBlob(IplImage* tmp_frame, IplImage* background,int id);

void parse(wxString fileName,float* ValuesVect,struct matrixDesc* MDSC);

IplImage* getFiltredBinaryImage(IplImage* currentImage, IplImage* backgroundImage, int value );

void drawBlob(IplImage * image, struct coordinate coord);
