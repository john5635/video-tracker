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

void execute(char * aviName,int id ){
	struct coordinate coord;
	
	IplImage* background = getBackground(aviName);
	
	CvMat* indexMat[7];

    int MP = 2; /* number of measurement vector dimensions */
    int DP = 4; /* number of state vector dimensions */
    int CP = 1; /* number of control vector dimensions */
	
	CvKalman* kalman = cvCreateKalman(DP,MP,CP);

	init(kalman, indexMat);
    
	CvMat* state=cvCreateMat(4,1,CV_32FC1);
	CvMat* measurement = cvCreateMat( 2, 1, CV_32FC1 );
    CvMat* process_noise = cvCreateMat(4, 1, CV_32FC1);
	
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
		coord = extractBlob(tmp_frame, background,id);
		
		if (coord.flag == false ) printf("No Blobs to extract"); 
		else{ 
			printf("Flag true!\n");
  			drawBlob(tmp_frame, coord);
			int Meanx, Meany;
			Meanx=(coord.Minx+coord.Maxx)/2;
			Meany=(coord.Miny+coord.Maxy)/2;
			cvmSet(measurement,0,0,Meanx);
			cvmSet(measurement,1,0,Meany);
			CvMat* u = cvCreateMat(1,1, CV_32FC1 );
			u->data.fl[0]=1;
			
			//Kalman Predict
			const CvMat* predict = cvKalmanPredict(kalman,u);
			cvMatMulAdd( kalman->measurement_matrix, state, measurement, measurement );

			//Kalman Correct
			const CvMat* correct= cvKalmanCorrect(kalman, measurement);
			cvMatMulAdd( kalman->transition_matrix, state, process_noise, state );
			
			float prx = predict->data.fl[0];
			float pry = predict->data.fl[1];
			float vx = predict->data.fl[2];
			float vy = predict->data.fl[3];
			printf("prx e' %f, pry è %f\n\n", prx, pry);
			float crx = correct->data.fl[0];
			float cry = correct->data.fl[1];
			float cvx = correct->data.fl[2];
			float cvy = correct->data.fl[3];
			printf("crx e' %f, cry è %f\n\n", crx, cry);


			//BISOGNEREBBE FAR SCRIVERE LE COORDINATE CORRETTE PER CONFRONTARE
			
			//run(kalman,coord);
			}	
		
		// display the image
		cvNamedWindow("image",1);
		cvShowImage("image", tmp_frame);
		
		// keep image 'til keypress
		cvWaitKey(0);
	}

	cvReleaseImage(&tmp_frame);
	cvDestroyWindow("image");
	cvReleaseCapture(&capture);
};


void drawBlob(IplImage * image, struct coordinate coord){

	int iMeanx, iMeany;
	iMeanx=(coord.Maxx+coord.Minx)/2;
	iMeany=(coord.Maxy+coord.Miny)/2;
	printf("Centro: x:%d, y:%d - - MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n-----------\n", iMeanx, iMeany,coord.Maxx,coord.Maxy,coord.Minx,coord.Miny);
	cvLine( image, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(255, 255 , 255), 4, 8, 0 );
	// mark box around blob
	cvRectangle( image, cvPoint(coord.Minx , coord.Miny ), cvPoint ( coord.Maxx, coord.Maxy ), CV_RGB(255, 255 , 255), 1, 8, 0);

}
