/*! \file condensation.cpp
 *
 *  \brief <b>This file contains the functions that implement the Condensation</b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 	and Nicola Martorana <martorana.nicola@gmail.com>
 *			and Marco Meoni <meonimarco@gmail.com>
 * 			This  code is distributed under the terms of <b>GNU GPL v2</b>
 *
 *  \version $Revision: 0.1 $
 *  \date 2006/10/27 
 *
 */
 
#include "condensation.h" 

CvConDensation* initCondensation ( CvMat** indexMat, int nSample, int maxWidth, int maxHeight ){
	
	int DP = indexMat[0]->cols; //! number of state vector dimensions */
	int MP = indexMat[2]->rows; //! number of measurement vector dimensions */

	CvConDensation* ConDens = cvCreateConDensation( DP, MP, nSample );
	CvMat* lowerBound;
	CvMat* upperBound;
	lowerBound = cvCreateMat(DP, 1, CV_32F);
	upperBound = cvCreateMat(DP, 1, CV_32F);
	
	cvmSet( lowerBound, 0, 0, 0.0 ); 
	cvmSet( upperBound, 0, 0, maxWidth );
	
	cvmSet( lowerBound, 1, 0, 0.0 ); 
	cvmSet( upperBound, 1, 0, maxHeight );

	cvmSet( lowerBound, 2, 0, 0.0 ); 
	cvmSet( upperBound, 2, 0, 0.0 );
	
	cvmSet( lowerBound, 3, 0, 0.0 ); 
	cvmSet( upperBound, 3, 0, 0.0 );
	//ConDens->DynamMatr = &indexMat[0]; fa il set della matrice del sistema
	

	for (int i=0;i<DP*DP;i++){
		ConDens->DynamMatr[i]= indexMat[0]->data.fl[i];
	}

	cvConDensInitSampleSet(ConDens, lowerBound, upperBound);
	
	CvRNG rng_state = cvRNG(0xffffffff);
	
	for(int i=0; i < nSample; i++){
		ConDens->flSamples[i][0] = cvRandInt( &rng_state ) % maxWidth; //0 represent the widht (x coord)
		ConDens->flSamples[i][1] = cvRandInt( &rng_state ) % maxHeight; //1 represent the height (1 coord)
	}
	
	//ConDens->DynamMatr=(float*)indexMat[0];
	//ConDens->State[0]=maxWidth/2;ConDens->State[1]=maxHeight/2;ConDens->State[2]=0;ConDens->State[3]=0;
	
	return ConDens;
}

coord updateCondensation ( CvConDensation* ConDens, coord Measurement, float * stdDX_ptr, float * stdDY_ptr){
	coord prediction;
	updateProcessProbDens(ConDens, Measurement, stdDX_ptr, stdDY_ptr);
	cvConDensUpdateByTime(ConDens);
	prediction.set(ConDens->State[0], ConDens->State[1]);
	return prediction;	
}

void updateProcessProbDens ( CvConDensation* ConDens, coord Measurement, float * stdDX_ptr, float * stdDY_ptr){	
	
	float ProbX, ProbY, var, stdDevX, stdDevY , varianceX, varianceY;
	
	ProbX=1; ProbY=1;
	
	sampleStat* statSampleX = new sampleStat (ConDens->SamplesNum);
	sampleStat* statSampleY = new sampleStat (ConDens->SamplesNum);
	
	//float stdev = sqrt(var/ConDens->SamplesNum);
	
	for(int i = 0; i < ConDens->SamplesNum; i++){
		statSampleX->setValue(ConDens->flSamples[i][0],i);
		statSampleY->setValue(ConDens->flSamples[i][1],i);
	}	
	
	stdDevX = statSampleX->getStdDeviation();
	stdDevY = statSampleY->getStdDeviation();

	varianceX = statSampleX->getVariance();
	varianceY = statSampleY->getVariance();

	for(int i = 0; i < ConDens->SamplesNum; i++){

		ProbX=1;
		ProbY=1;
		
		ProbX*= (float) exp( -1 * (Measurement.cX - ConDens-> flSamples[i][0]) * (Measurement.cX-ConDens-> flSamples[i][0]) / ( 2 * varianceX ) );
		
		ProbY*= (float) exp( -1  * (Measurement.cY - ConDens-> flSamples[i][1]) * (Measurement.cY-ConDens-> flSamples[i][1]) / (2 * varianceY ) ) ;
		
		ConDens->flConfidence[i] = ProbX*ProbY;
	}

	*stdDX_ptr = stdDevX;
	*stdDY_ptr = stdDevY;
	//printf("\nstdDXcondens:%f\nstdDYcondens:%f",stdDevX,stdDevY);
}
