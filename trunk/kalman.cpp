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

void init(CvKalman * kalman, CvMat** indexMat){
	
	struct matrixDesc MDSC[7];
	std::vector<float> ValuesVect;
	parse("./data.txt",&ValuesVect,MDSC);

	
	for (int i=0;i<2;i++){
		indexMat[i] = cvCreateMat( MDSC[i].nRows, MDSC[i].nCols, CV_32FC1 );
		}

	//indexMat[0] = cvCreateMat( 4, 4, CV_32FC1 ); /* state transition matrix (A) */
	//indexMat[1] = cvCreateMat( 4, 1, CV_32FC1 ); /* control matrix (Bu)(it is not used if there is no control)*/
	//indexMat[2] = cvCreateMat( 2, 4, CV_32FC1 ); /* measurement matrix (H) */
	//indexMat[3] = cvCreateMat( 4, 4, CV_32FC1 ); /* process noise covariance matrix (Q) */
	//indexMat[4] = cvCreateMat( 2, 2, CV_32FC1 ); /* measurement noise covariance matrix (R) */
	//indexMat[5] = cvCreateMat( 4, 4, CV_32FC1 ); /* posteriori error estimate covariance matrix P(0)*/
	//indexMat[6] = cvCreateMat( 4, 1, CV_32FC1 ); /* corrected state (x(0)) */
	//indexMat[7] = cvCreateMat( rowA, colA, CV_32FC1 ); //???
	
	
	
	for (int i=0;i<2;i++){
		for (int j=0;j<indexMat[i]->rows;j++){
			for (int l=0;l<indexMat[i]->cols;l++){
				indexMat[i][(j*indexMat[i]->cols)+l].data.fl = 0;//qui ci si skianta i val contenuti nel valVect 
			}
		}	

	}

}


void execute(CvKalman* kalman, char * aviName ){
	struct coordinate* coord;
	IplImage* background = getBackground(aviName);
	
	int height = background->height;
	int width = background->width;
	int channels = background->nChannels;


	CvCapture* capture = cvCaptureFromAVI(aviName);
  
	if( !capture ) {
   		fprintf( stderr, "ERROR: capture is NULL \n" );
   		exit(0);
  	}

	IplImage* tmp_frame = cvQueryFrame(capture); //current image in the cycles
 
 
	if(!tmp_frame) {
         fprintf( stderr, "ERROR: Bad video\n" );
         exit(0);
        }

	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){


		coord = extractBlob(tmp_frame, background);
		if (coord->flag == false ) break;

	}




}; // Predict and Correct
