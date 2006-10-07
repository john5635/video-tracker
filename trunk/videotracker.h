#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>
#include <iostream>

using namespace std;

struct coordinate { // coordinate of the tracked object in this case a ball.
	int* cc;
	int* cr;
	float radius;
	bool flag;
} ;


void init(CvKalman * kalman, CvMat** indexMat);
void execute(CvKalman* kalman, char * aviName );

float* getValue();


IplImage* getBackground(char* aviName);

struct coordinate* extractBlob(IplImage* tmp_frame, IplImage* background);
