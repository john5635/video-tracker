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
	lowerBound = cvCreateMat(2, 1, CV_32F);
	upperBound = cvCreateMat(2, 1, CV_32F);
	
	cvmSet( lowerBound, 0, 0, 0.0 ); cvmSet( upperBound, 0, 0, maxWidth );
	cvmSet( lowerBound, 1, 0, 0.0 ); cvmSet( upperBound, 1, 0, maxHeight );
	
	cvConDensInitSampleSet(ConDens, lowerBound, upperBound);
	
	for(int i=0; i < nSample; i++){
		ConDens->flSamples[i][0]+=maxWidth/2;
		ConDens->flSamples[i][1]+=maxHeight/2;
	}
	
	ConDens->DynamMatr=(float*)indexMat[0];
	
	return ConDens;
}

coord updateCondensation ( CvConDensation* ConDens, coord Coord){
	
	
}

void updateProcessProbDens ( CvConDensation* ConDens, coord Measurement){	
	
	float Prob, var;
	
	float stdev = sqrt(var/ConDens->SamplesNum);
	
	for(int i = 0; i < ConDens->SamplesNum; i++){
		Prob=1;
		Prob*=(float)exp(-stdev * (Measurement.cX - ConDens->flSamples[i][0])*(Measurement.cX-ConDens->flSamples[i][0]));
		Prob*=(float)exp(-stdev * (Measurement.cY - ConDens->flSamples[i][1])*(Measurement.cY-ConDens->flSamples[i][1]));
		ConDens->flConfidence[i] = Prob;
	}
	
}
