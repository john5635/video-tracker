/*! \file kalman.cpp
 *
 *
 * \brief <b>This file contains the two function that implement kalamn: the init where the kalman structure are fill up with the data taken by the parser </b>
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
 
#include "videotracker.h"

///The function that fill up the kalman structure parsing a data file, where there is the matrices
/**
 * \brief  the map of the kalman matrix in the indexMat vector
 *			0 -> A
 *			1 -> Bu
 *			2 -> H
 *			3 -> Q
 *			4 -> R
 *			5 -> P
 *			6 -> NULL			
 * \param kalman the pointer to the kalman structure
 * \param indexMat the pointer to the vector of matrix where will be the parsed value
 */

void init(CvKalman * kalman, CvMat** indexMat){
	
	struct matrixDesc *MDSC = new struct matrixDesc[5];
	
	float ValuesVect[100];

	//!parsing the file
	parse(_("./data.txt"),ValuesVect,MDSC);

	
	for (int i=0;i< NUMBER_OF_MATRIX -1 ;i++){
		indexMat[i] = cvCreateMat( MDSC[i].nRows, MDSC[i].nCols, CV_32FC1 );
		}

	int h = 0;
	
	//! gettin the value from the ValuesVect and store in indexMat
	for (int i=0;i< NUMBER_OF_MATRIX -1;i++){
		for (int j=0;j<indexMat[i]->rows;j++){
			for (int l=0;l<indexMat[i]->cols;l++){
				indexMat[i]->data.fl[j*indexMat[i]->cols+l] = ValuesVect[h];
				h++;
			}
		}	

	}
	
	//!filling up the kalamn structure. For now I have left the copyMat method. The other method is more efficent however.
	copyMat(indexMat[0], kalman->transition_matrix);//A
	copyMat(indexMat[1], kalman->control_matrix);//Bu
	copyMat(indexMat[2], kalman->measurement_matrix);;//H
	copyMat(indexMat[3], kalman->process_noise_cov);//Q
	copyMat(indexMat[4], kalman->measurement_noise_cov);//R
	copyMat(indexMat[5], kalman->error_cov_pre);//P

	//!Here we must set the initial state
	double a[] = { 100,  100,  0,  0};

	CvMat Ma=cvMat(1, 4, CV_32FC1, a);
	copyMat(&Ma, kalman->state_post);
	

}

///The function that make a copy from a matrix to another one
/**
 * \param source The source Matrix
 * \param dest The Destination  Matrix, where the values are stored.
 */

void copyMat (CvMat* source, CvMat* dest){
	int i,j;
	for (i=0; i<source->rows; i++)
		for (j=0; j<source->cols;j++)
			dest->data.fl[i*source->cols+j]=source->data.fl[i*source->cols+j];

}


/// \todo:THIS FUCTION MUST BECAME updateKalman not run:in that must be predict and correct
/*
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
*/
