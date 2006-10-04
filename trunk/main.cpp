#include <cvaux.h>  // include core library interface
#include <highgui.h> // include GUI library interface
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>

 
 
 
 //this is a sample for foreground detection functions
int main(int argc, char ** argv) {
 
     //  char fn[100];
 
     IplImage*       tmp_frame = NULL;
 
     CvCapture*      cap = NULL;
 
      /* sprintf(fn, "L:/pub/mva05.avi");
       sprintf(fn, "L:/incoming/Comics/Suika/Suika 01.avi"); // 

       sprintf(fn, "L:/incoming/Comics/Suika/Suika 03.avi");
       sprintf(fn, "L:/incoming/Comics/Suika/Suika 04.avi"); 
       sprintf(fn, "L:/incoming/output.avi"); // cinepak success
 //    sprintf(fn, "D:/programming/1 results/result  video/bear_h.avi"); // MS-CRAM fail
 //    sprintf(fn, "I:/first kh.avi"); // Dvix fail
 
 */
     cap = cvCaptureFromAVI(argv[1]);
 
     tmp_frame = cvQueryFrame(cap);
 
 
     if(!tmp_frame) {
         printf("bad video \n");
         exit(0);
     }
 
     cvNamedWindow("BG", 1);
 
     cvNamedWindow("FG", 1);
 
     //create BG model
 
   //CvBGStatModel* bg_model = cvCreateFGDStatModel(tmp_frame);
 
    CvBGStatModel* bg_model = cvCreateGaussianBGModel(tmp_frame);
 
     for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), 
fr++ )
     {
         printf("frame# %d \t", fr);  
 
         double t = (double)cvGetTickCount();
 
         cvUpdateBGStatModel(tmp_frame, bg_model);
         
       t = (double)cvGetTickCount() - t;
         printf( "updating time : %.1f milli seconds \n", t/
 (cvGetTickFrequency()*1000.) );
 
         cvShowImage("BG", bg_model->background);
         cvShowImage("FG", bg_model->foreground);
 
         int k = cvWaitKey(5);
         if( k == 27 ) break;
     }
 
     cvReleaseBGStatModel( &bg_model );
     cvReleaseCapture(&cap);
 
     cvDestroyWindow("BG");
     cvDestroyWindow("FG");
 
     return 0;
 }
