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
 
struct coordinate extractBlob(CBlobResult blobs, struct coordinate selectedCoord){
   
	struct coordinate coord;
	CBlob Blob;

	if ( blobs.GetNumBlobs()==0 ) {
		coord.flag=false; 
		return coord;
	}
	else {
		
		//!Get the blob info
		Blob = getNearestBlob( blobs, selectedCoord);
		
		//!Creating the coordinate struct
		coord.Maxx= (int ) Blob.MaxX();
		coord.Maxy= (int ) Blob.MaxY();
		coord.Minx= (int ) Blob.MinX();
		coord.Miny= (int ) Blob.MinY();
		coord.flag=true; 
		
		return coord;
	}

}

void drawInitialBlobs(IplImage * tmp_frame, CBlobResult blobs){

	
	struct coordinate drawCoord;

	for (int i=0; i<blobs.GetNumBlobs();i++){
		
		//!Creating the coordinate struct
		drawCoord.Maxx= (int ) blobs.GetBlob(i).MaxX();
		drawCoord.Maxy= (int ) blobs.GetBlob(i).MaxY();
		drawCoord.Minx= (int ) blobs.GetBlob(i).MinX();
		drawCoord.Miny= (int ) blobs.GetBlob(i).MinY();
		drawCoord.flag=true; 

		drawBlob(tmp_frame, drawCoord, 255, 255, 0);
	}
}

CBlob getNearestBlob(CBlobResult blobs, struct coordinate coord){
	
	int tot = blobs.GetNumBlobs();
	int Meanx, Meany, tempMeanx, tempMeany;
	CBlob Blob;
	float distance[10]; // 10 è il numero massimo di blob trovabile in un video
	//for (int j=0; j<10; j++){distance[j]=0;}
	float minimum;

	Meanx=(coord.Minx+coord.Maxx)/2;
	Meany=(coord.Miny+coord.Maxy)/2;
	struct coordinate tempCoord;
	
	//Questo ciclo for fa la distanza manhattan tra le coordinate passate e tutti i blob catturati e crea il vettore con tutte le distanze.
	for (int i=0; i<tot; i++){
		Blob = blobs.GetBlob(i);
		tempCoord.Maxx= (int ) Blob.MaxX();
		tempCoord.Maxy= (int ) Blob.MaxY();
		tempCoord.Minx= (int ) Blob.MinX();
		tempCoord.Miny= (int ) Blob.MinY();
		tempMeanx=(tempCoord.Minx+tempCoord.Maxx)/2;
		tempMeany=(tempCoord.Miny+tempCoord.Maxy)/2;
		distance[i] = sqrt((double)(tempMeanx - Meanx)*(tempMeanx - Meanx) + (tempMeany - Meany)*(tempMeany - Meany));
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
   	
	struct coordinate coord;
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
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 60);
	
	//!This two row of code are to filter the blob find from the library by a bug that match ablob like all the 	image and return the center of it
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (tmp_frame->height)*(tmp_frame->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (tmp_frame->height)+(tmp_frame->width)*2*0.8);
	
	//! Create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );
	//return blobs;

	return blobs;
}





