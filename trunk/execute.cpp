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
	 
	//Declare the variable of Kalman
	struct coordinate coordReal;
	struct coordinate coordPredict;
	coordPredict.flag=true;
	CvMat* indexMat[NUMBER_OF_MATRIX];
	float * predict = NULL;
	
	//!vision of the avi file
	cvNamedWindow( "image", 1 );
	char code = -1;
	
	//Declare the structure for the background subtraction
	CvGaussBGStatModelParams paramMoG;
	CvBGStatModel *bkgdMdl = NULL;
	IplImage * binaryBackground = NULL;

	bool selected = false;
	
	CvKalman* kalman = NULL;

	CvMat* state = NULL;
	CvMat* measurement = NULL;
	CvMat* process_noise = NULL;


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

	initBackgroundModel(&bkgdMdl,tmp_frame, &paramMoG);
/*
	
	selectedCoord = extractBlob( tmp_frame, binBack, selectedCoord);

	CvKalman* kalman = initKalman(indexMat, selectedCoord);

	CvMat* state=cvCreateMat(kalman->DP,1,CV_32FC1);
	CvMat* measurement = cvCreateMat( kalman->MP, 1, CV_32FC1 );
        CvMat* process_noise = cvCreateMat(kalman->DP, 1, CV_32FC1);
	
	coordReal=selectedCoord;
	*/
		
	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){
		
		binaryBackground = updateBackground(bkgdMdl,tmp_frame);
		
		if (getNumBlob(tmp_frame,binaryBackground)>0){
			
			if (selected == false){
				//!Extact and draw all blobs
				drawInitialBlobs(tmp_frame, binaryBackground);
				
				//Questa è la simulazione del click del marto
				struct coordinate selectedCoord;
				selectedCoord.Maxx= 0;
				selectedCoord.Maxy= 20;
				selectedCoord.Minx= 120;
				selectedCoord.Miny= 7;
				selectedCoord.flag= true;
				selectedCoord = extractBlob( tmp_frame, binaryBackground, selectedCoord);
				
				kalman = initKalman(indexMat, selectedCoord);

				state=cvCreateMat(kalman->DP,1,CV_32FC1);
				measurement = cvCreateMat( kalman->MP, 1, CV_32FC1 );
				process_noise = cvCreateMat(kalman->DP, 1, CV_32FC1);
	
				coordReal=selectedCoord;
				
				selected=true;

			}
			
			else {
				coordReal = extractBlob(tmp_frame, binaryBackground, coordReal);
				
				printf("Flag true!\n");
				
				drawBlob(tmp_frame, coordReal, 255,255,255);
				
				//!updateKalman functions that provied to estimate with Kalman filter
				predict = updateKalman(kalman,state,measurement,process_noise,coordReal);
				
				//!computing the coordinate predict from Kalman, the X one.
				coordPredict.Maxx = (int) predict[0] + (coordReal.Maxx - coordReal.Minx)/2;
				coordPredict.Minx = (int) predict[0] - (coordReal.Maxx - coordReal.Minx)/2;
				
				//!computing the coordinate predict from Kalman, the Y one.
				coordPredict.Maxy = (int) predict[1] + (coordReal.Maxy - coordReal.Miny)/2;
				coordPredict.Miny = (int) predict[1] - (coordReal.Maxy - coordReal.Miny)/2;
				
				coordPredict.flag = true;
				
				drawBlob(tmp_frame, coordPredict, 0, 255, 0);
			}

		}
		
		else {
			if (coordPredict.flag == true) {
								coordReal=coordPredict;} 
		}
	
		//! display the image
		cvShowImage("image", tmp_frame);
		
		//!showing in loop all frames
		code = (char) cvWaitKey( 100 );
		if( code > 0 )
                break;
		
		//! keep image 'til keypress
		//cvWaitKey(5);

	}

	cvReleaseImage(&tmp_frame);
	cvDestroyWindow("image");
	cvReleaseCapture(&capture);
};



///The function that draw the blob in a image
/**
 * \param image the image where the blob must be drawn
 * \param struct coordinate the coordinate of the blob to plot
 * \param R the RED components of RGB color. 
 * \param G the GREEN components of RGB color.
 * \param B the BLUE components of RGB color.
*/

void drawBlob (IplImage * image, struct coordinate coord, int R, int G, int B){

	int iMeanx, iMeany;
	iMeanx=(coord.Maxx+coord.Minx)/2;
	iMeany=(coord.Maxy+coord.Miny)/2;
	
	//!printing the center and other coordinate
	printf("Centro: x:%d, y:%d - - MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n-----------\n", iMeanx, iMeany,coord.Maxx,coord.Maxy,coord.Minx,coord.Miny);
	
	cvLine( image, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(R, G , B), 4, 8, 0 );
	
	// mark box around blob
	cvRectangle( image, cvPoint(coord.Minx , coord.Miny ), cvPoint ( coord.Maxx, coord.Maxy ), CV_RGB(R, G , B), 1, 8, 0);

}
void initBackgroundModel(CvBGStatModel ** bgmodel, IplImage* tmp_frame, CvGaussBGStatModelParams* paramMoG){
	
	paramMoG->win_size = 200; //200;
	paramMoG->n_gauss = 3; //5;
	paramMoG->bg_threshold = 0.1; //0.7;
	paramMoG->std_threshold = 5; //2.5;
	paramMoG->minArea = 200.f; //15.f;
	paramMoG->weight_init = 0.01; //0.05;
	paramMoG->variance_init = 30; //30*30;
	*bgmodel = cvCreateGaussianBGModel(tmp_frame, paramMoG);
	
}
