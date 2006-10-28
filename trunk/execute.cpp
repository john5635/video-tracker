/*! \file execute.cpp
 *
 *
 * \brief <b>Main stream of the program: here are called creatKalman,initKalman,extractBlob and the updateKalman functions</b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 	and Nicola Martorana <martorana.nicola@gmail.com>
 *			and Marco Meoni <meonimarco@gmail.com>
 * 	
 * This  code is distributed under the terms of <b>GNU GPL v2</b>
 *
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
 *
 * \version $Revision: 0.1 $
 * \date 2006/10/27 
 * 
 *
 *
 */
 
 
#include "videotracker.h" 


///The function that execute the main stream of the program
/**
 * \param aviName the name of the avi video to process
 * \param id the numbero of the blob to take in the extractBlob functions
 */

void execute(char * aviName,int id ){
	
	struct coordinate coord;
	
	IplImage* background = getBackground(aviName);
	
	CvMat* indexMat[NUMBER_OF_MATRIX];

    int MP = 2; //! number of measurement vector dimensions */
    int DP = 4; //! number of state vector dimensions */
    int CP = 1; //! number of control vector dimensions */
	
	CvKalman* kalman = cvCreateKalman(DP,MP,CP);

	initKalman(kalman, indexMat);
    
	CvMat* state=cvCreateMat(4,1,CV_32FC1);
	CvMat* measurement = cvCreateMat( 2, 1, CV_32FC1 );
    CvMat* process_noise = cvCreateMat(4, 1, CV_32FC1);
	
	CvCapture* capture = cvCaptureFromAVI(aviName);
  
	if( !capture ) {
   		fprintf( stderr, "ERROR: capture is NULL \n" );
   		exit(0);
  	}

	IplImage* tmp_frame = cvQueryFrame(capture); //!current image in the cycles
 
	if(!tmp_frame) {
         fprintf( stderr, "ERROR: Bad video\n" );
         exit(0);
        }

	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){
		coord = extractBlob(tmp_frame, background,id);
		
		if (coord.flag == false ) printf("No Blobs to extract"); 
		else{ 
			printf("Flag true!\n");
  			drawBlob(tmp_frame, coord);
			
			//!updateKalman functions that provied to estimate with Kalman filter
			updateKalman(kalman,state,measurement,process_noise,coord);

		}	
		
		//! display the image
		cvNamedWindow("image",1);
		cvShowImage("image", tmp_frame);
		
		//! keep image 'til keypress
		cvWaitKey(0);
	}

	cvReleaseImage(&tmp_frame);
	cvDestroyWindow("image");
	cvReleaseCapture(&capture);
};



///The function that draw the blob in a image
/**
 * \param image the image where the blob must be drawn
 * \param struct coordinate the coordinate of the blob to plot
 */

void drawBlob(IplImage * image, struct coordinate coord){

	int iMeanx, iMeany;
	iMeanx=(coord.Maxx+coord.Minx)/2;
	iMeany=(coord.Maxy+coord.Miny)/2;
	
	//!printing the center and other coordinate
	printf("Centro: x:%d, y:%d - - MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n-----------\n", iMeanx, iMeany,coord.Maxx,coord.Maxy,coord.Minx,coord.Miny);
	
	cvLine( image, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(255, 255 , 255), 4, 8, 0 );
	
	// mark box around blob
	cvRectangle( image, cvPoint(coord.Minx , coord.Miny ), cvPoint ( coord.Maxx, coord.Maxy ), CV_RGB(255, 255 , 255), 1, 8, 0);

}
