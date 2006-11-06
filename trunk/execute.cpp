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
	 

	struct coordinate coordReal;
	struct coordinate coordPredict;
	
	//IplImage* background = getBackground(aviName);
	//da riscrivere la funzione getBackground(...);
	//in prima istanza c'è da fargli fare la media sui primi n frames e avere un primo Background


	//!getting the binary background VA BENE
	IplImage* tempBack = cvCreateImage(cvGetSize(background),IPL_DEPTH_8U,1);
	cvCvtColor(background, tempBack, CV_RGB2GRAY);
	IplImage* binBack = cvCreateImage(cvGetSize(background),IPL_DEPTH_8U,1);
	if(!cvSaveImage("tempBack.jpg",tempBack)) printf("Could not save the backgroundimage\n");
	cvThreshold(tempBack,binBack,100,255,CV_THRESH_BINARY);
	if(!cvSaveImage("binBack.jpg",binBack)) printf("Could not save the backgroundimage\n");
	
	CvMat* indexMat[NUMBER_OF_MATRIX];

	
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

	//qui c'è da far ritornare il vettore dei blob contenuti nel primo frame
	
	//poi si richiama la funzione drawBlob su tutti i Blobs di questo frame
	//visualizzato il frame con i blobs, si blocca l'esecuzione in attesa del click
	//cvSetMouseCallback(...) contenuta in HighGUI permette di settare l'azione da fare
	//in relazione all'evento del mouse vedi nei samples "ffilldemo.c"
	//all'interno della CallBack è possibile ottenere il valore del punto in cui si è clikkato
	//NB relativo all'immagine e non alla finestra (-:

	//Creation and initializzation of Kalman	
	//il filtro di Kalman va inizializzato con le coordinate del blob selezionato
	
	CvKalman* kalman = initKalman(indexMat);

	CvMat* state=cvCreateMat(kalman->DP,1,CV_32FC1);
	CvMat* measurement = cvCreateMat( kalman->MP, 1, CV_32FC1 );
    CvMat* process_noise = cvCreateMat(kalman->DP, 1, CV_32FC1);
	

	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){
		
	//ogni n frames facciamo l'aggiornamento del BackGround tipo backGroundUpdate(backgroundBINARIO!)

		//invece dell'Id è necessario passare le coordinate del blob che ci interessa alla funzione extractBlob
		//la funzione restituisce le coordinate del blob che si trova più vicino
		coordReal = extractBlob(tmp_frame, binBack,id);

		if (coordReal.flag == false ) printf("No Blobs to extract"); 
		else{ 
			printf("Flag true!\n");
  			drawBlob(tmp_frame, coordReal, 255,255,255);
			
			//!updateKalman functions that provied to estimate with Kalman filter
			float * predict = updateKalman(kalman,state,measurement,process_noise,coordReal);
			
			//!computing the coordinate predict from Kalman, the X one.
			coordPredict.Maxx = (int) predict[0] + (coordReal.Maxx - coordReal.Minx)/2;
			coordPredict.Minx = (int) predict[0] - (coordReal.Maxx - coordReal.Minx)/2;
			
			//!computing the coordinate predict from Kalman, the Y one.
			coordPredict.Maxy = (int) predict[1] + (coordReal.Maxy - coordReal.Miny)/2;
			coordPredict.Miny = (int) predict[1] - (coordReal.Maxy - coordReal.Miny)/2;
			
			drawBlob(tmp_frame, coordPredict, 0, 255, 0);
			
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
 * \param R the RED components of RGB color. 
 * \param G the GREEN components of RGB color.
 * \param B the BLUE components of RGB color.
*/

void drawBlob(IplImage * image, struct coordinate coord, int R, int G, int B){

	int iMeanx, iMeany;
	iMeanx=(coord.Maxx+coord.Minx)/2;
	iMeany=(coord.Maxy+coord.Miny)/2;
	
	//!printing the center and other coordinate
	printf("Centro: x:%d, y:%d - - MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n-----------\n", iMeanx, iMeany,coord.Maxx,coord.Maxy,coord.Minx,coord.Miny);
	
	cvLine( image, cvPoint(iMeanx, iMeany), cvPoint(iMeanx, iMeany), CV_RGB(R, G , B), 4, 8, 0 );
	
	// mark box around blob
	cvRectangle( image, cvPoint(coord.Minx , coord.Miny ), cvPoint ( coord.Maxx, coord.Maxy ), CV_RGB(R, G , B), 1, 8, 0);

}
