#ifndef  _EXTRACTBLOB_H
#define  _EXTRACTBLOB_H


/*! \file execute.h
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


//Includes for the cvBlob library header
#include "Blob.h"
#include "BlobResult.h"
#include "videotracker.h"
#include "execute.h"
///Other Define




///Functions Declaration

coord extractBlob(CBlobResult blobs, coord selectedCoord);

void drawInitialBlobs(IplImage * tmp_frame, CBlobResult blobs);

CBlob getNearestBlob(CBlobResult blobs, coord coordinate);

CBlobResult getBlobs(IplImage* tmp_frame, IplImage* binBack);


#endif
