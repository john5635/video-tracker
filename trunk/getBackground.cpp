/*! \file getBackground.cpp
 *
 *
 * \brief <b>The functions that provides make a bakground subtracktion of the video</b>
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
 * \date 2006/10/7 
 * 
 *
 *
 */

#include "videotracker.h"


///The function that make the Background subtraction with Gaussian model
/**
 * \param aviName the name of the avi video to process
 * \return savedBackgroundImage the background of the video
 */

IplImage* updateBackground(CvBGStatModel *bg_model, IplImage * tmp_frame){
	 
	//Updating the Gaussian Model
	cvUpdateBGStatModel(tmp_frame, bg_model);
    if(!cvSaveImage("background.jpg",bg_model->background)) printf("Could not save the backgroundimage\n");
	
	//!getting the binary background
	IplImage* tempBack = cvCreateImage(cvGetSize(bg_model->background),IPL_DEPTH_8U,1);
	cvCvtColor(bg_model->background, tempBack, CV_RGB2GRAY);
	IplImage* binBack = cvCreateImage(cvGetSize(bg_model->background),IPL_DEPTH_8U,1);
	if(!cvSaveImage("tempBack.jpg",tempBack)) printf("Could not save the backgroundimage\n");
	cvThreshold(tempBack,binBack,100,255,CV_THRESH_BINARY);
	if(!cvSaveImage("binBack.jpg",binBack)) printf("Could not save the backgroundimage\n");
	
	//returing the binary background
	return binBack; 
}
