#ifndef  _GETBACKGROUND_H
#define  _GETBACKGROUND_H

/*! \file getBackground.h
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


///Other Define

#include "videotracker.h"


///Functions Declaration

IplImage* updateBackground(CvBGStatModel * bg_model, IplImage * tmp_frame);

void initBackgroundModel(CvBGStatModel ** , IplImage* , CvGaussBGStatModelParams* );

IplImage * getBinaryImage(IplImage * image);

#endif
