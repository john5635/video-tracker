

#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>

#define rowA 2
#define colA 1

int main (int argc, char ** argv){

	CvMat* indexMat[8];
	
	indexMat[0] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[1] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[2] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[3] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[4] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[5] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[6] = cvCreateMat( rowA, colA, CV_32FC1 );
	indexMat[7] = cvCreateMat(10, 2, CV_32FC1);

	int MP,DP,CP;
	
	CvKalman* kalman = cvCreateKalman(DP,MP,CP);
	

	init_Kalman(kalman, indexMat);
	execute(kalman, argv[1]);





return 0;
}
