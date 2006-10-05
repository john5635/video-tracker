/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>

#define rowA 2
#define colA 1

#MEO BUHOOO

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
