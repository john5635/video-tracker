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

#include "videotracker.h"

#define rowA 2
#define colA 1

int main (int argc, char ** argv){

	CvMat* indexMat[8];
	
	indexMat[0] = cvCreateMat( rowA, colA, CV_32FC1 ); /* predicted state (x'(k)):x(k)=A*x(k-1)+B*u(k) */
	indexMat[1] = cvCreateMat( rowA, colA, CV_32FC1 ); /* corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k)) */
	indexMat[2] = cvCreateMat( rowA, colA, CV_32FC1 ); /* state transition matrix (A) */
	indexMat[3] = cvCreateMat( rowA, colA, CV_32FC1 ); /* control matrix (B)(it is not used if there is no control)*/
	indexMat[4] = cvCreateMat( rowA, colA, CV_32FC1 ); /* measurement matrix (H) */
	indexMat[5] = cvCreateMat( rowA, colA, CV_32FC1 ); /* process noise covariance matrix (Q) */
	indexMat[6] = cvCreateMat( rowA, colA, CV_32FC1 ); /* measurement noise covariance matrix (R) */
	indexMat[7] = cvCreateMat(10, 2, CV_32FC1); 

    int MP; /* number of measurement vector dimensions */
    int DP; /* number of state vector dimensions */
    int CP; /* number of control vector dimensions */
	
	CvKalman* kalman = cvCreateKalman(DP,MP,CP);

	init_Kalman(kalman, indexMat);
	execute(kalman, argv[1]);


	return 0;
}
