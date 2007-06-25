/*! \file execute.cpp
 *
 *
 *  \brief <b>Main stream of the program: here are called creatKalman,initKalman,extractBlob and the updateKalman functions</b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 		and Nicola Martorana <martorana.nicola@gmail.com>
 *				and Marco Meoni <meonimarco@gmail.com>
 * 				This  code is distributed under the terms of <b>GNU GPL v2</b>
 * 
 *  \version $Revision: 0.1 $
 *  \date 2006/10/27 
 * 
 *
 *
 */
 
 
#include "execute.h" 


///The function that execute the main stream of the program
/**
 * \param aviName the name of the avi video to process
 * \param id the numbero of the blob to take in the extractBlob functions
 */
int CLICK[2];


void execute(char * aviName,int id ){

	int MOD;
	bool ENABLE;

	ENABLE=true;
	MOD = 1;
	 
	//Declare the variable of Kalman
	coord coordReal;
	coord candidateCoordReal;
	coord coordPredict;
	coord predictConDens;
	//coordPredict.flag=true;
	CvMat* indexMat[NUMBER_OF_MATRIX];
	float * predict = NULL;
	CBlobResult blobsVector;
	float distance; 
	//!vision of the avi file
	cvNamedWindow( "video-tracker", 1 );
	char code = -1;
	int maxNumFrame = 0;

	//Declare the structure for the background subtraction
	CvGaussBGStatModelParams paramMoG;
	CvBGStatModel *bkgdMdl = NULL;
	IplImage * binaryForeground = NULL;

	bool selected = false;
	bool firstTime = true;
	
	CvKalman* kalman = NULL;
	CvConDensation* ConDens = NULL;
	CvMat* state = NULL;
	CvMat* measurement = NULL;
	CvMat* process_noise = NULL;

	FILE * realCFile;
	FILE * kalmanFile;
	realCFile = fopen("coordinateReali.txt","w");
	kalmanFile = fopen("coordinateKalman.txt","w");
	FILE * condenseFile;
	condenseFile = fopen("coordinateCondensation.txt","w");
	FILE * distanceKalmanFILE;
	distanceKalmanFILE = fopen ("distanzaKalman.txt","w");
	FILE * distanceCondenseFILE;
	distanceCondenseFILE = fopen ("distanzaCondensation.txt","w");
	FILE * resultFile;
	resultFile = fopen("Risultati.txt","w");
	//ellipse declarations
	double theta;
	CvSize axes;
	int muX, muY, condenseFrame;
	condenseFrame=0;
	muX=0;
	muY=0;
	//Variance for drawing condensation ellipse
	float stdDXcondens, stdDYcondens, meanStdDXcondens, meanStdDYcondens, resultDistance, meanKalmanDistance, meanCondenseDistance ;
	meanKalmanDistance=0;
	meanCondenseDistance=0;
	meanStdDXcondens=0;
	meanStdDYcondens=0;
	stdDXcondens=0;
	stdDYcondens=0;
	CvSize axesCondens;

	theta=0;
	
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
	int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
// 	printf("H:%d, W:%d", frameH, frameW);
	initBackgroundModel(&bkgdMdl,tmp_frame, &paramMoG);
	
	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){
		
		printf("\n-----------\nFrame #%d",fr);
		binaryForeground = updateBackground(bkgdMdl,tmp_frame);
		blobsVector = getBlobs(tmp_frame,binaryForeground);

		if ( blobsVector.GetNumBlobs()>0 ){
			if (selected == false){
				//!Extact and draw all blobs
				drawInitialBlobs(tmp_frame, blobsVector);
				//cvNamedWindow("video-tracker",0);
				CLICK[0]=0;
				CLICK[1]=0;
				cvSetMouseCallback( "video-tracker", on_mouse, 0);
				
				cvShowImage("video-tracker", tmp_frame);
				drawInitialBlobs(tmp_frame, blobsVector);
				
				cvWaitKey(7000);
				
				coord selectedCoord;
				selectedCoord.set(CLICK[0],CLICK[1]);
				selectedCoord = extractBlob( blobsVector, selectedCoord);
				
				//Init Kalman
				kalman = initKalman(indexMat, selectedCoord);
				//Init Condensation
				ConDens = initCondensation (indexMat, 1000, frameW, frameH);
				
				state=cvCreateMat(kalman->DP,1,CV_32FC1);
				measurement = cvCreateMat( kalman->MP, 1, CV_32FC1 );
				process_noise = cvCreateMat(kalman->DP, 1, CV_32FC1);
	
				coordReal=selectedCoord;
				coordPredict=coordReal;
				muX=frameW/2;
				muY=frameH/2;
				selected=true;
			}
			else {
				if (MOD == 1){			
					//coordReal.set(candidateCoordReal.MaxX, candidateCoordReal.MinX, candidateCoordReal.MaxY, candidateCoordReal.MinY);
				 
					//printf("Flag true!\n");
					coordReal = extractBlob( blobsVector, coordReal);
	
					
					drawBlob(tmp_frame, coordReal, 255,255,255);
					
					//!updateKalman functions that provied to estimate with Kalman filter
					predict = updateKalman(kalman,coordReal);
					
					coordPredict.set (coordReal.MaxX, coordReal.MinX, coordReal.MaxY, coordReal.MinY);
					coordPredict.set ((int)predict[0], (int)predict[1]);
					
					
					//!updateCondensation function.
					predictConDens = updateCondensation(ConDens, coordReal, &stdDXcondens, &stdDYcondens);					
					
					axesCondens = cvSize(stdDXcondens, stdDYcondens);
					meanStdDXcondens+=stdDXcondens;
					meanStdDYcondens+=stdDYcondens;
					condenseFrame++;
					
					//!drawing the ellipse Initial State. Should be Fixed.
					//theta = 0;//(180/PI)*atan((float)coordPredict.lY/(float)coordPredict.lX);


					cvEllipse( tmp_frame, cvPoint(predictConDens.cX,predictConDens.cY), axesCondens, theta, 0, 360, CV_RGB(0,255,0),1);
					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(0,255, 0), 1, 8, 0 );										//!draw condense prediction
  					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(predictConDens.cX,predictConDens.cY), CV_RGB(0,255, 0), 4, 8, 0 );

					muX = sqrt(kalman->error_cov_pre->data.fl[0]);
					muY = sqrt(kalman->error_cov_pre->data.fl[5]);
					
					axes = cvSize( muX , muY );
					
					cvEllipse( tmp_frame, cvPoint(coordPredict.cX,coordPredict.cY), axes, theta, 0, 360, CV_RGB(255,0,0),1);
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordPredict.cX,coordPredict.cY), CV_RGB(255,0, 0), 4, 8, 0 );
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(255,0, 0), 1, 8, 0 );
				
					//print coord
					//real					
					fprintf(realCFile,"%.0f,%.0f \n",coordReal.cX,coordReal.cY);
					printf("\nBlob center:    (x:%d, y:%d)", (int)coordReal.cX, (int)coordReal.cY);
					//kalman
					fprintf(kalmanFile,"%.0f,%.0f \n",coordPredict.cX,coordPredict.cY);
					printf("\nKalman center:  (x:%d, y:%d)", (int)predict[0],(int)predict[1]);
					//condensation
					fprintf(condenseFile,"%.0f,%.0f \n",predictConDens.cX,predictConDens.cY);
					printf("\nConDens center: (x:%d, y:%d)\n", (int)predictConDens.cX,(int)predictConDens.cY);
					
					//distance from real
					resultDistance = sqrt((double)(coordReal.cX - coordPredict.cX)*(coordReal.cX - coordPredict.cX) + (coordReal.cY - coordPredict.cY)*(coordReal.cY - coordPredict.cY));
					meanKalmanDistance+=resultDistance;
					fprintf(distanceKalmanFILE,"%.0f\n",resultDistance);
					
					resultDistance = sqrt((double)(coordReal.cX - predictConDens.cX)*(coordReal.cX - predictConDens.cX) + (coordReal.cY - predictConDens.cY)*(coordReal.cY - predictConDens.cY));
					meanCondenseDistance+=resultDistance;
					fprintf(distanceCondenseFILE,"%.0f\n",resultDistance);
				}
				else{
					if (fr%MOD == 0){

				firstTime=false;
				//distance = sqrt((double)(candidateCoordReal.cX - coordReal.cX)*(candidateCoordReal.cX - coordReal.cX) + (candidateCoordReal.cY - coordReal.cY)*(candidateCoordReal.cY - coordReal.cY));
				

				if ( is_in_the_ellipse(coordReal,coordPredict,muX,muY) || ENABLE){
					//coordReal.set(candidateCoordReal.MaxX, candidateCoordReal.MinX, candidateCoordReal.MaxY, candidateCoordReal.MinY);
				 
					//printf("Flag true!\n");
					
					drawBlob(tmp_frame, coordReal, 255,255,255);
					
					//!updateKalman functions that provied to estimate with Kalman filter
					predict = updateKalman(kalman,coordReal);
					
					coordPredict.set (coordReal.MaxX, coordReal.MinX, coordReal.MaxY, coordReal.MinY);
					coordPredict.set ((int)predict[0], (int)predict[1]);
					
					
					//!updateCondensation function.
					predictConDens = updateCondensation(ConDens, coordReal, &stdDXcondens, &stdDYcondens);					
					
					axesCondens = cvSize(stdDXcondens, stdDYcondens);
					meanStdDXcondens+=stdDXcondens;
					meanStdDYcondens+=stdDYcondens;
					condenseFrame++;
					
					//!drawing the ellipse Initial State. Should be Fixed.
					//theta = 0;//(180/PI)*atan((float)coordPredict.lY/(float)coordPredict.lX);


					cvEllipse( tmp_frame, cvPoint(predictConDens.cX,predictConDens.cY), axesCondens, theta, 0, 360, CV_RGB(0,255,0),1);
					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(0,255, 0), 1, 8, 0 );										//!draw condense prediction
  					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(predictConDens.cX,predictConDens.cY), CV_RGB(0,255, 0), 4, 8, 0 );

					muX = sqrt(kalman->error_cov_pre->data.fl[0]);
					muY = sqrt(kalman->error_cov_pre->data.fl[5]);
					
					axes = cvSize( muX , muY );
					
					cvEllipse( tmp_frame, cvPoint(coordPredict.cX,coordPredict.cY), axes, theta, 0, 360, CV_RGB(255,0,0),1);
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordPredict.cX,coordPredict.cY), CV_RGB(255,0, 0), 4, 8, 0 );
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(255,0, 0), 1, 8, 0 );
				
					//print coord
					//real					
					fprintf(realCFile,"%.0f,%.0f \n",coordReal.cX,coordReal.cY);
					printf("\nBlob center:    (x:%d, y:%d)", (int)coordReal.cX, (int)coordReal.cY);
					//kalman
					fprintf(kalmanFile,"%.0f,%.0f \n",coordPredict.cX,coordPredict.cY);
					printf("\nKalman center:  (x:%d, y:%d)", (int)predict[0],(int)predict[1]);
					//condensation
					fprintf(condenseFile,"%.0f,%.0f \n",predictConDens.cX,predictConDens.cY);
					printf("\nConDens center: (x:%d, y:%d)\n", (int)predictConDens.cX,(int)predictConDens.cY);
					
					//distance from real
					resultDistance = sqrt((double)(coordReal.cX - coordPredict.cX)*(coordReal.cX - coordPredict.cX) + (coordReal.cY - coordPredict.cY)*(coordReal.cY - coordPredict.cY));
					meanKalmanDistance+=resultDistance;
					fprintf(distanceKalmanFILE,"%.0f\n",resultDistance);
					
					resultDistance = sqrt((double)(coordReal.cX - predictConDens.cX)*(coordReal.cX - predictConDens.cX) + (coordReal.cY - predictConDens.cY)*(coordReal.cY - predictConDens.cY));
					meanCondenseDistance+=resultDistance;
					fprintf(distanceCondenseFILE,"%.0f\n",resultDistance);
				}
				else {
					//coordReal.set(candidateCoordReal.MaxX, candidateCoordReal.MinX, candidateCoordReal.MaxY, candidateCoordReal.MinY);
				 
					//printf("Flag true!\n");
					
					drawBlob(tmp_frame, coordReal, 255,255,255);
					
					//!updateKalman functions that provied to estimate with Kalman filter
					//predict = updateKalman(kalman,coordReal);
					
					coordPredict.set (coordReal.MaxX, coordReal.MinX, coordReal.MaxY, coordReal.MinY);
					coordPredict.set ((int)predict[0], (int)predict[1]);
					
					
					//!updateCondensation function.
					predictConDens = updateCondensation(ConDens, coordReal, &stdDXcondens, &stdDYcondens);					
					
					axesCondens = cvSize(stdDXcondens, stdDYcondens);
					meanStdDXcondens+=stdDXcondens;
					meanStdDYcondens+=stdDYcondens;
					condenseFrame++;
					
					//!drawing the ellipse Initial State. Should be Fixed.
					//theta = 0;//(180/PI)*atan((float)coordPredict.lY/(float)coordPredict.lX);


					cvEllipse( tmp_frame, cvPoint(predictConDens.cX,predictConDens.cY), axesCondens, theta, 0, 360, CV_RGB(0,255,0),1);
					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(0,255, 0), 1, 8, 0 );										//!draw condense prediction
  					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(predictConDens.cX,predictConDens.cY), CV_RGB(0,255, 0), 4, 8, 0 );

					muX = sqrt(kalman->error_cov_pre->data.fl[0]);
					muY = sqrt(kalman->error_cov_pre->data.fl[5]);
					
					axes = cvSize( muX , muY );
					
					cvEllipse( tmp_frame, cvPoint(coordPredict.cX,coordPredict.cY), axes, theta, 0, 360, CV_RGB(255,0,0),1);
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordPredict.cX,coordPredict.cY), CV_RGB(255,0, 0), 4, 8, 0 );
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(255,0, 0), 1, 8, 0 );
				
					//print coord
					//real					
					fprintf(realCFile,"%.0f,%.0f \n",coordReal.cX,coordReal.cY);
					printf("\nBlob center:    (x:%d, y:%d)", (int)coordReal.cX, (int)coordReal.cY);
					//kalman
					fprintf(kalmanFile,"%.0f,%.0f \n",coordPredict.cX,coordPredict.cY);
					printf("\nKalman center:  (x:%d, y:%d)", (int)predict[0],(int)predict[1]);
					//condensation
					fprintf(condenseFile,"%.0f,%.0f \n",predictConDens.cX,predictConDens.cY);
					printf("\nConDens center: (x:%d, y:%d)\n", (int)predictConDens.cX,(int)predictConDens.cY);
					
					//distance from real
					resultDistance = sqrt((double)(coordReal.cX - coordPredict.cX)*(coordReal.cX - coordPredict.cX) + (coordReal.cY - coordPredict.cY)*(coordReal.cY - coordPredict.cY));
					meanKalmanDistance+=resultDistance;
					fprintf(distanceKalmanFILE,"%.0f\n",resultDistance);
					
					resultDistance = sqrt((double)(coordReal.cX - predictConDens.cX)*(coordReal.cX - predictConDens.cX) + (coordReal.cY - predictConDens.cY)*(coordReal.cY - predictConDens.cY));
					meanCondenseDistance+=resultDistance;
					fprintf(distanceCondenseFILE,"%.0f\n",resultDistance);
				}
			}
					else{
					//axesCondens = cvSize(stdDXcondens, stdDYcondens);
					//axes = cvSize( muX , muY );

					coordReal = extractBlob( blobsVector, coordReal);
					drawBlob(tmp_frame, coordReal, 255,255,255);

					if(!firstTime){
					cvEllipse( tmp_frame, cvPoint(predictConDens.cX,predictConDens.cY), axesCondens, theta, 0, 360, CV_RGB(0,255,0),1);
					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(0,255, 0), 1, 8, 0 );
					cvLine( tmp_frame,  cvPoint(predictConDens.cX,predictConDens.cY), cvPoint(predictConDens.cX,predictConDens.cY), CV_RGB(0,255, 0), 4, 8, 0 );
					cvEllipse( tmp_frame, cvPoint(coordPredict.cX,coordPredict.cY), axes, theta, 0, 360, CV_RGB(255,0,0),1);
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordPredict.cX,coordPredict.cY), CV_RGB(255,0, 0), 4, 8, 0 );
					cvLine( tmp_frame,  cvPoint(coordPredict.cX,coordPredict.cY), cvPoint(coordReal.cX,coordReal.cY), CV_RGB(255,0, 0), 1, 8, 0 );
					}	
				}
				 }
			}
		}
		else {
			if ( (coordPredict.flag == true)) {
				coordReal.set(coordPredict.MaxX, coordPredict.MinX, coordPredict.MaxY, coordPredict.MinY);
				predict = updateKalman (kalman,coordReal);
				//coordPredict.set (coordReal.MaxX, coordReal.MinX, coordReal.MaxY, coordReal.MinY);
				coordPredict.set ((int)predict[0], (int)predict[1]);
				drawBlob(tmp_frame, coordPredict, 255, 0, 0);
			} 
		}
	
		//! display the image
		cvShowImage("video-tracker", tmp_frame);
		
		//!showing in loop all frames
		code = (char) cvWaitKey( 100 );
// 		if( code > 0 )
//                 break;
		
		//! keep image 'til keypress
		//cvWaitKey(5);

	}
	//print mean_Std-Dev
	meanStdDXcondens=meanStdDXcondens/condenseFrame;
	meanStdDYcondens=meanStdDYcondens/condenseFrame;
	meanKalmanDistance=meanKalmanDistance/condenseFrame;
	meanCondenseDistance=meanCondenseDistance/condenseFrame;
	fprintf(resultFile,"Varianza Media per il condensation \nVarX: %.0f \nVarY: %.0f\n",meanStdDXcondens, meanStdDYcondens);
	fprintf(resultFile,"\nDistanza Media tra la previsione di Kalman e il blob reale: %.0f\n",meanKalmanDistance);
	fprintf(resultFile,"\nDistanza Media tra la previsione del Condensation e il blob reale: %.0f\n",meanCondenseDistance);
	fclose(realCFile);
	fclose(kalmanFile);
	fclose(condenseFile);
	fclose(distanceKalmanFILE);
	fclose(distanceCondenseFILE);
	fclose(resultFile);
	cvReleaseImage(&tmp_frame);
	cvDestroyWindow("video-tracker");
	cvReleaseCapture(&capture);
	
}



///The function that draw the blob in a image
/**
 * \param image the image where the blob must be drawn
 * \param struct coordinate the coordinate of the blob to plot
 * \param R the RED components of RGB color. 
 * \param G the GREEN components of RGB color.
 * \param B the BLUE components of RGB color.
*/

void drawBlob (IplImage * image, coord CcB, int R, int G, int B){
	
	//!printing the center and other coordinate
			       
	
	//printf("Centro: x:%f, y:%f - - MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n-----------\n", CcB.cX, CcB.cY, CcB.MaxX, CcB.MaxY, CcB.MinX, CcB.MinY);
	
	cvLine( image,  cvPoint( (int)CcB.cX, (int) CcB.cY),  cvPoint( (int) CcB.cX,  (int)CcB.cY), CV_RGB(R, G , B), 4, 8, 0 );
	
	// mark box around blob
	//cvRectangle( image, cvPoint(CcB.MinX , CcB.MinY ), cvPoint ( CcB.MaxX, CcB.MaxY ), CV_RGB(R, G , B), 1, 8, 0);

}

void on_mouse( int event, int x, int y, int flags, void* param ){
 
	switch( event ){
		case CV_EVENT_LBUTTONDOWN:{
		CLICK[0]=x;
		CLICK[1]=y;
		}break;
	}
}

bool is_in_the_ellipse(coord P, coord O, int a, int b){
	double c = sqrt(double(a*a)-(b*b));
	double dist = sqrt((P.cX-O.cX+c)*(P.cX-O.cX+c)+(P.cY-O.cY)*(P.cY-O.cY))+sqrt((P.cX-O.cX-c)*(P.cX-O.cX-c)+(P.cY-O.cY)*(P.cY-O.cY));
	if (dist<=(2*a)) return true;
	else return false;

}
