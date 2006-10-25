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


void init(CvKalman * kalman, CvMat** indexMat){
	
	struct matrixDesc *MDSC = new struct matrixDesc[5];
	
	//CvKalman * kalman=ptr;
	float ValuesVect[100];

	parse(_("./data.txt"),ValuesVect,MDSC);

	
	for (int i=0;i<6;i++){
		indexMat[i] = cvCreateMat( MDSC[i].nRows, MDSC[i].nCols, CV_32FC1 );
		}

	//indexMat[0] = cvCreateMat( 4, 4, CV_32FC1 ); /* state transition matrix (A) */
	//indexMat[1] = cvCreateMat( 4, 1, CV_32FC1 ); /* control matrix (Bu)(it is not used if there is no control)*/
	//indexMat[2] = cvCreateMat( 2, 4, CV_32FC1 ); /* measurement matrix (H) */
	//indexMat[3] = cvCreateMat( 4, 4, CV_32FC1 ); /* process noise covariance matrix (Q) */
	//indexMat[4] = cvCreateMat( 2, 2, CV_32FC1 ); /* measurement noise covariance matrix (R) */
	//indexMat[5] = cvCreateMat( 4, 4, CV_32FC1 ); /* posteriori error estimate covariance matrix P(0)*/
	//indexMat[6] = cvCreateMat( 4, 1, CV_32FC1 ); /* corrected state (x(0)) */
	//indexMat[7] = cvCreateMat( rowA, colA, CV_32FC1 ); //???
	
	
	int h = 0;

	for (int i=0;i<6;i++){
		for (int j=0;j<indexMat[i]->rows;j++){
			for (int l=0;l<indexMat[i]->cols;l++){
				indexMat[i]->data.fl[j*indexMat[i]->cols+l] = ValuesVect[h];//qui ci si skianta i val contenuti nel valVect 
				h++;
			}
		}	

	}
	
	/*CvRandState rng;
    //cvRandInit( &rng, 0, 1, -1, CV_RAND_UNI );
	//cvRandSetRange( &rng, 0, 0.1, 0 );
    rng.disttype = CV_RAND_NORMAL;


	cvRand( &rng, kalman->state_post );
	*/
	
		
	//copying the data into Kalman Structure

	kalman->transition_matrix=indexMat[0];//A
	copyMat(indexMat[1], kalman->control_matrix);//Bu
	copyMat(indexMat[2], kalman->measurement_matrix);;//H
	copyMat(indexMat[3], kalman->process_noise_cov);//Q
	copyMat(indexMat[4], kalman->measurement_noise_cov);//R
	copyMat(indexMat[5], kalman->error_cov_pre);//P

	//QUI BISOGNEREBBE FARE IL SET SULLA MATRICE DI STATO INIZIALE
	double a[] = { 100,  100,  0,  0};

	CvMat Ma=cvMat(1, 4, CV_32FC1, a);
	copyMat(&Ma, kalman->state_post);
	//cvZero(kalman->state_post);


	/*memcpy(kalman->transition_matrix->data.fl, indexMat[0], sizeof(indexMat[0]));//, sizeof(indexMat[0])); //A
	kalman->control_matrix= cvCloneMat(indexMat[1]);//, sizeof(indexMat[1]));  //B
	kalman->measurement_matrix= cvCloneMat(indexMat[2]);//, sizeof(indexMat[2])); //H
	kalman->process_noise_cov=cvCloneMat(indexMat[3]);//, sizeof(indexMat[3])); //Q
	kalman->measurement_noise_cov=cvCloneMat(indexMat[4]);//, sizeof(indexMat[4])); //R
	kalman->temp1->data.fl[0]=indexMat[5]->data.fl[0]; //cvCloneMat(indexMat[5]);//, sizeof(indexMat[5])); //u
	kalman->error_cov_pre=cvCloneMat(indexMat[6]);//, sizeof(indexMat[6])); //P
	*/
  //delete MDSC;
}

void copyMat (CvMat* source, CvMat* dest){
	int i,j;
	for (i=0; i<source->rows; i++)
		for (j=0; j<source->cols;j++)
			dest->data.fl[i*source->cols+j]=source->data.fl[i*source->cols+j];

}

void run(CvKalman * kalman, struct coordinate coord){
	
	CvMat* measurement = cvCreateMat( 2, 1, CV_32FC1 );
	int Meanx, Meany;
	Meanx=(coord.Minx+coord.Maxx)/2;
	Meany=(coord.Miny+coord.Maxy)/2;
	cvmSet(measurement,0,0,Meanx);
	cvmSet(measurement,1,0,Meany);
	
	CvMat* state=cvCreateMat(4,1,CV_32FC1);
	
	CvMat* u = cvCreateMat(1,1, CV_32FC1 );
	u->data.fl[0]=1;
    CvMat* process_noise = cvCreateMat(2, 1, CV_32FC1);

	
	//Kalman Predict
	const CvMat* predict = cvKalmanPredict(kalman,u);
	cvMatMulAdd( kalman->measurement_matrix, state, measurement, measurement );

	
	float prx = predict->data.fl[0];
	float pry = predict->data.fl[1];
	float vx = predict->data.fl[2];
	float vy = predict->data.fl[3];
	
	printf("prx è %f, pry è %f\n\n", prx, pry);
	
	
	//Kalman Correct
	const CvMat* correct= cvKalmanCorrect(kalman, measurement);
	cvMatMulAdd( kalman->transition_matrix, state, process_noise, state );

	float crx = correct->data.fl[0];
	float cry = correct->data.fl[1];
	float cvx = correct->data.fl[2];
	float cvy = correct->data.fl[3];
	printf("crx è %f, cry è %f\n\n", crx, cry);
	}

//void execute(CvKalman* kalman, char * aviName ){
	//La parte ora commentata è in findBlob
        /*struct coordinate coord;
	IplImage* background = getBackground(aviName);
	
	int height = background->height;
	int width = background->width;
	int channels = background->nChannels;


	CvCapture* capture = cvCaptureFromAVI(aviName);
  
	if( !capture ) {
   		fprintf( stderr, "ERROR: capture is NULL \n" );
   		exit(0);
  	}

	IplImage* tmp_frame = cvQueryFrame(capture); //current image in the cycles
 
 
	if(!tmp_frame) {
         fprintf( stderr, "ERROR: Bad video\n" );
         exit(0);
        }

	for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(capture), fr++ ){


		coord = extractBlob(tmp_frame, background);
		if (coord.flag == false ) break; else printf("Flag true!");
		printf("MaxX: %d, MaxY: %d, MinX: %d, MinY: %d\n", coord.Maxx,coord.Maxy,coord.Minx,coord.Miny);
	}*/




//}; // Predict and Correct
