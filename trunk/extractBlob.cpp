/*! \file extractBlob.cpp
 *
 *
 *  \brief <b>This file contains the function that aim to catch the blob in the video using the cvBlob library using opencv. The taken blob from the pool is that provied by the ID passed throught the function </b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 	and Nicola Martorana <martorana.nicola@gmail.com>
 *			and Marco Meoni <meonimarco@gmail.com>
 * 			This  code is distributed under the terms of <b>GNU GPL v2</b>
 *
 *  \version $Revision: 0.1 $
 *  \date 2006/10/27 
 * 
 *
 *
 */



#include "extractBlob.h"

///The function that extract the Blob form the image and return the coordinate of the one extracted.
/**
 * \param tmp_frame The image from which extract the blob.
 * \param background The background image of the avifile to obatin the foreground object to track
 * \param id  The id of the blob to take
 * \return struct coordinate The coordinate of the extracted blob.
 */
 
coord extractBlob(CBlobResult blobs, coord selectedCoord){
   
	coord coordinate;
	CBlob Blob;

	if ( blobs.GetNumBlobs()==0 ) {
		coordinate.flag=false; 
		return coordinate;
	}
	else {
		
		//!Get the blob info
		Blob = getNearestBlob( blobs, selectedCoord);
		
		//!Creating the coordinate struct
		coordinate.set( (int) Blob.MaxX(), (int) Blob.MinX(), (int) Blob.MaxY(), (int) Blob.MinY());
		
		return coordinate;
	}

}

void drawInitialBlobs(IplImage * tmp_frame, CBlobResult blobs){

	
	coord drawCoord;

	for (int i=0; i<blobs.GetNumBlobs();i++){
		
		//!Creating the coordinate struct
		drawCoord.set( (int) blobs.GetBlob(i).MaxX(), (int) blobs.GetBlob(i).MinX(), (int) blobs.GetBlob(i).MaxY(), (int) blobs.GetBlob(i).MinY());

		drawBlob(tmp_frame, drawCoord, 255, 255, 0);
	}
}

CBlob getNearestBlob(CBlobResult blobs, coord coordinate){
	
	int tot = blobs.GetNumBlobs();
	CBlob Blob;
	float distance[10]; // 10 è il numero massimo di blob trovabile in un video
	float minimum;
	
	coord tempCoord;

	//Questo ciclo for fa la distanza manhattan tra le coordinate passate e tutti i blob catturati e crea il vettore con tutte le distanze.
	for (int i=0; i<tot; i++){
		Blob = blobs.GetBlob(i);
		tempCoord.set( (int) Blob.MaxX(), (int) Blob.MinX(), (int) Blob.MaxY(), (int) Blob.MinY());
		distance[i] = sqrt((double)(tempCoord.cX - coordinate.cX)*(tempCoord.cX - coordinate.cX) + (tempCoord.cY - coordinate.cY)*(tempCoord.cY - coordinate.cY));
	}

	int minDistanceId=0;
	
	//Questo ciclo for becca la minima distanza fra tutte quelle calcolate
	for (int j=0; j<tot; j++){
		minimum = min( distance[j], distance[minDistanceId]);	
		if ( distance[j] == minimum ) minDistanceId = j;
		}

	//Ottenuta la minima distanza si va a ritornare il Blob corrispondente
	Blob = blobs.GetBlob( minDistanceId );
	//delete[] distance;
	return Blob;

}

CBlobResult getBlobs(IplImage* tmp_frame, IplImage* binBack){
 
	IplImage* binFore = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
		
	//!get the binary foreground object
	cvSub( getBinaryImage(tmp_frame) , binBack, binFore, NULL );
	if(!cvSaveImage("binFore.jpg",binFore)) printf("Could not save the backgroundimage\n");

	//!Starting the extracting of Blob
	CBlobResult blobs;
	
	//! get the blobs from the image, with no mask, using a threshold of 100
	blobs = CBlobResult( binFore, NULL, 10, true );
	
	//! Create a file with all the found blob
	blobs.PrintBlobs( "blobs.txt" );

	//! discard the blobs with less area than 60 pixels
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 40);
	
	//!This two row of code are to filter the blob find from the library by a bug that match ablob like all the 	image and return the center of it
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (tmp_frame->height)*(tmp_frame->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (tmp_frame->height)+(tmp_frame->width)*2*0.8);
	
	//! Create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );
	//return blobs;

	return blobs;
}
