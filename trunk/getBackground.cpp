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

IplImage* getBackground(char* aviName){
	 
	 IplImage* tmp_frame = NULL;
	 IplImage* savedBackgroundImage;
	 CvCapture* cap = NULL;
	// cvNamedWindow("BG", 1); da attivare se si passa un opzione al programma -v che sta per visualizza video
	 
	 cap = cvCaptureFromAVI(aviName);
     	 tmp_frame = cvQueryFrame(cap);
 
   	  if(!tmp_frame) {
       		  printf("Bad video \n");
       		  exit(0);
    	 }

	 CvBGStatModel* bg_model = cvCreateGaussianBGModel(tmp_frame);

    	 for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ ){
        	printf("Reading avi %s\nframe# %d \t\n", aviName, fr);
		double t = (double)cvGetTickCount();
		cvUpdateBGStatModel(tmp_frame, bg_model);
        	printf( "updating time : %.1f milli seconds \n", t/ (cvGetTickFrequency()*1000.) );
		// cvShowImage("BG", bg_model->background);da attivare se si passa un opzione al programma -v che sta per visualizza video
     		savedBackgroundImage = cvCloneImage(bg_model->background);
		int k = cvWaitKey(5);
        	if( k == 27 ) break;
	 }
	 if(!cvSaveImage("background.jpg",savedBackgroundImage)) printf("Could not save the backgroundimage\n");
	 cvReleaseBGStatModel( &bg_model );
         cvReleaseCapture(&cap);
   	 //cvDestroyWindow("BG");da attivare se si passa un opzione al programma -v che sta per visualizza video
	 return savedBackgroundImage;
}
