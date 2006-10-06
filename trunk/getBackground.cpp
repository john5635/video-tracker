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

IplImage* getBackground(char* aviName){
	 
	 IplImage* tmp_frame = NULL;
	 CvCapture* cap = NULL;
	 
	 cap = cvCaptureFromAVI(aviName);
     tmp_frame = cvQueryFrame(cap);
 
     if(!tmp_frame) {
         printf("bad video \n");
         exit(0);
     }

	 CvBGStatModel* bg_model = cvCreateGaussianBGModel(tmp_frame);

     for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ ){
        
		cvUpdateBGStatModel(tmp_frame, bg_model);
        
		//cvShowImage("BG", bg_model->background);
        //cvShowImage("FG", bg_model->foreground);
     
	 }

	 cvReleaseBGStatModel( &bg_model );
     cvReleaseCapture(&cap);
	 return 0;
}