/*! \file extractBlob.cpp
 *
 *
 * \brief <b>This file contains the function that aim to catch the blob in the video using the cvBlob library using opencv. The taken blob from the pool is that provied by the ID passed throught the function </b>
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

//Includes for the cvBlob library header
#include "Blob.h"
#include "BlobResult.h"

#include "videotracker.h"

///The function that extract the Blob form the image and return the coordinate of the one extracted.
/**
 * \param tmp_frame The image from which extract the blob.
 * \param background The background image of the avifile to obatin the foreground object to track
 * \param id  The id of the blob to take
 * \return struct coordinate The coordinate of the extracted blob.
 */
 
struct coordinate extractBlob(IplImage* tmp_frame, IplImage* binBack,struct coordinate selectedCoord){
   
	struct coordinate coord;
	
	//!getting the binary current frame
	IplImage* img = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvCvtColor(tmp_frame, img, CV_RGB2GRAY);
	IplImage* binImg = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	IplImage* binFore = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvThreshold(img,binImg,100,255,CV_THRESH_BINARY);
	if(!cvSaveImage("binImg.jpg",binImg)) printf("Could not save the backgroundimage\n");

	//!get the binary foreground object
	cvSub( binImg, binBack, binFore, NULL );
	if(!cvSaveImage("binFore.jpg",binFore)) printf("Could not save the backgroundimage\n");

	//!Starting the extracting of Blob
	CBlobResult blobs;
	
	//! get the blobs from the image, with no mask, using a threshold of 100
	blobs = CBlobResult( binFore, NULL, 10, true );
	
	//! Create a file with all the found blob
	blobs.PrintBlobs( "blobs.txt" );

	//! discard the blobs with less area than 60 pixels
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 60);
	
	//!This two row of code are to filter the blob find from the library by a bug that match ablob like all the image and return the center of it
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (img->height)*(img->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (img->height)+(img->width)*2*0.8);
	
	//! Create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );

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

CBlobResult extractBlob(IplImage* tmp_frame, IplImage* binBack){
   	
	struct coordinate coord;
	
	//!getting the binary current frame
	IplImage* img = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvCvtColor(tmp_frame, img, CV_RGB2GRAY);
	IplImage* binImg = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	IplImage* binFore = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvThreshold(img,binImg,100,255,CV_THRESH_BINARY);
	if(!cvSaveImage("binImg.jpg",binImg)) printf("Could not save the backgroundimage\n");

	//!get the binary foreground object
	cvSub( binImg, binBack, binFore, NULL );
	if(!cvSaveImage("binFore.jpg",binFore)) printf("Could not save the backgroundimage\n");

	//!Starting the extracting of Blob
	CBlobResult blobs;
	
	//! get the blobs from the image, with no mask, using a threshold of 100
	blobs = CBlobResult( binFore, NULL, 10, true );
	
	//! Create a file with all the found blob
	blobs.PrintBlobs( "blobs.txt" );

	//! discard the blobs with less area than 60 pixels
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 60);
	
	//!This two row of code are to filter the blob find from the library by a bug that match ablob like all the image and return the center of it
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (img->height)*(img->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (img->height)+(img->width)*2*0.8);
	
	//! Create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );
	return blobs;
}

CBlob getNearestBlob(CBlobResult blobs, struct coordinate coord){
	int tot = blobs.GetNumBlobs();
	int Meanx, Meany, tempMeanx, tempMeany;
	CBlob Blob;
	float* distance = NULL; 
	float minimum;
	distance = new float[tot];
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
		distance[i] = fabs(tempMeanx - Meanx) + fabs(tempMeany - Meany);
	}
	int minDistanceId=0;
	
	//Questo ciclo for becca la minima distanza fra tutte quelle calcolate
	for (int j=0; j<tot; j++){
		minimum = min( distance[j], distance[minDistanceId]);	
		if ( distance[j] == minimum ) minDistanceId = j;
	}
	//Ottenuta la minima distanza si va a ritornare il Blob corrispondente
	Blob = blobs.GetBlob( minDistanceId );
	delete[] distance;
	return Blob;

}
struct coordinate extractBlob(IplImage* tmp_frame, IplImage* binBack,int id){
   
	struct coordinate coord;
	
	//!getting the binary current frame
	IplImage* img = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvCvtColor(tmp_frame, img, CV_RGB2GRAY);
	IplImage* binImg = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	IplImage* binFore = cvCreateImage(cvGetSize(tmp_frame),IPL_DEPTH_8U,1);
	cvThreshold(img,binImg,100,255,CV_THRESH_BINARY);
	if(!cvSaveImage("binImg.jpg",binImg)) printf("Could not save the backgroundimage\n");

	//!get the binary foreground object
	cvSub( binImg, binBack, binFore, NULL );
	if(!cvSaveImage("binFore.jpg",binFore)) printf("Could not save the backgroundimage\n");

	//!Starting the extracting of Blob
	CBlobResult blobs;
	
	//! get the blobs from the image, with no mask, using a threshold of 100
	blobs = CBlobResult( binFore, NULL, 10, true );
	
	//! Create a file with all the found blob
	blobs.PrintBlobs( "blobs.txt" );

	//! discard the blobs with less area than 60 pixels
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 60);
	
	//!This two row of code are to filter the blob find from the library by a bug that match ablob like all the image and return the center of it
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, (img->height)*(img->width)*0.8);
	blobs.Filter( blobs, B_INCLUDE, CBlobGetPerimeter(), B_LESS, (img->height)+(img->width)*2*0.8);
	
	//! Create a file with filtered results
	blobs.PrintBlobs( "filteredBlobs.txt" );

	CBlob Blob;

	if ( blobs.GetNumBlobs()==0 ) {
		coord.flag=false; 
		return coord;
	}
	else {
		
		//!Get the blob info
		Blob = blobs.GetBlob(id);
		
		//!Creating the coordinate struct
		coord.Maxx= (int ) Blob.MaxX();
		coord.Maxy= (int ) Blob.MaxY();
		coord.Minx= (int ) Blob.MinX();
		coord.Miny= (int ) Blob.MinY();
		coord.flag=true; 
		
		return coord;
	}

}
