#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <wx/utils.h>
#include <wx/wx.h>
#include <wx/string.h>

using namespace std;

int main(int argc, char *argv[]) {

#ifdef WIN32
	CvCapture* capture = cvCaptureFromAVI("sphere1.avi");
#else
	CvCapture* capture = cvCaptureFromAVI(argv[1]);
#endif

	if( !capture ) {
		//fprintf( stderr, "ERROR: capture is NULL \n" );
		cerr<<"Error:Caputre is null"<<endl;
		getchar();
		return -1;
	}

	// Create a window in which the captured images will be presented
	cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );

	// Show the image captured from the camera in the window and repeat
	int k=0;
	char filesave[50];
	//bool flag = false;
	while( true ) {
		k++;
		// Get one frame
		IplImage* frame = cvQueryFrame( capture );
		if( !frame ) {
			//fprintf( stderr, "There is no frame to get. Press Any Key\n" );
			cerr<<"There is no frame to get. Press Any Key"<<endl;
			getchar();
			//flag=true;
			break;
			}			
		IplImage* img = 0;
		cvShowImage( "mywindow", frame );
		img=cvRetrieveFrame(capture);

//----------------------CODICE CHE USA LE WX---------------------------
		


#ifdef WIN32
		wxString dir = wxString("./data"); 
		if (!wxDirExists(dir.c_str() )){ 
			wxMkdir(dir);}	
#else

		const wxChar* dir=_("./data");
		if (! wxDirExists(dir)){
			if (! wxMkdir(dir)){
			cout<<"Cartella non creata"<<endl;}}
#endif		

//---------------------------------------------------------------------------------

		sprintf(filesave,"./data/immagine%d.jpg",k);
				
		if(!cvSaveImage(filesave,img)) cout<<"Could not save the image"<<endl;
				
		// Do not release the frame!
		//If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
		//remove higher bits using AND operator
		if( (cvWaitKey(10) & 255) == 27 ) break;
	
		}

	// Release the capture device housekeeping
	cvReleaseCapture( &capture );
	cvDestroyWindow( "mywindow" );
	//getchar();
	return 0;
}
