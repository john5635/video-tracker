#ifndef _KALMAN_H
#define _KALMAN_H

/*! \file kalman.h
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 		and Nicola Martorana <martorana.nicola@gmail.com>
 *				and Marco Meoni <meonimarco@gmail.com>
 * 				This  code is distributed under the terms of <b>GNU GPL v2</b>
 * 
 *  \version $Revision: 0.1 $
 *  \date 2006/10/27 
 * 
 *
 *
 */


///Includes

#include "videotracker.h"
#include "parser.h"
#include "Coord.h"

///Other Define




///Functions Declaration

CvKalman* initKalman(CvMat** , coord );

void copyMat (CvMat* , CvMat* );

float* updateKalman(CvKalman *, coord );

float* updateKalman(CvKalman *);


#endif
