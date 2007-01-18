/*! \file getBackground.cpp
 *
 *
 *  \brief <b>The functions that provides make a bakground subtracktion of the video</b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 		    and Nicola Martorana <martorana.nicola@gmail.com>
 *				    and Marco Meoni <meonimarco@gmail.com>
 *				    This  code is distributed under the terms of <b>GNU GPL v2</b>
 *  \version $Revision: 0.1 $
 *  \date 2006/10/7 
 * 
 *
 *
 */

#include "getBackground.h"




void initBackgroundModel(CvBGStatModel ** bgmodel, IplImage* tmp_frame, CvGaussBGStatModelParams* paramMoG){
	
	paramMoG->win_size = 200; //200;
	paramMoG->n_gauss = 3; //5;
	paramMoG->bg_threshold = 0.1; //0.7;
	paramMoG->std_threshold = 5; //2.5;
	paramMoG->minArea = 200.f; //15.f;
	paramMoG->weight_init = 0.01; //0.05;
	paramMoG->variance_init = 30; //30*30;
	*bgmodel = cvCreateGaussianBGModel(tmp_frame, paramMoG);
	
}


///The function that make the Background subtraction with Gaussian model
/**
 * \param aviName the name of the avi video to process
 * \return savedBackgroundImage the background of the video
 */

IplImage* updateBackground(CvBGStatModel *bg_model, IplImage * tmp_frame){
	 
	//Updating the Gaussian Model
	cvUpdateBGStatModel(tmp_frame, bg_model);
        if(!cvSaveImage("./data/background.jpg",bg_model->background)) printf("Could not save the background image\n");
	if(!cvSaveImage("./data/foreground.jpg",bg_model->foreground)) printf("Could not save the foreground image\n");
	
	//returing the binary background
	return bg_model->foreground;
}

IplImage * getBinaryImage(IplImage * image){


	//!getting the binary current frame
	IplImage* img = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvCvtColor(image, img, CV_RGB2GRAY);
	IplImage* binImg = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	cvThreshold(img,binImg,100,255,CV_THRESH_BINARY);
	//if(!cvSaveImage("binImg.jpg",binImg)) printf("Could not save the backgroundimage\n");
	cvReleaseImage(&img);
	return binImg;

}

