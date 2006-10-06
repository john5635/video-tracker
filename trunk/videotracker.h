#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>
//#include <kalman.h>

typedef struct coordinate{ // coordinate of the tracked object in this case a ball.
	int* cc;
	int* cr;
	float radius;
	bool flag;
};


void init_Kalman(CvKalman * kalman, CvMat** indexMat);
void execute(CvKalman* kalman, char * aviName );

IplImage* getBackground(char* aviName);

struct coordinate* extractBlob(IplImage* tmp_frame, IplImage* background);

IplImage*  getBackground(char * aviName);
