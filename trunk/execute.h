#ifndef  _EXECUTE_H
#define  _EXECUTE_H


/*! \file execute.h
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


///Other Define
#include "videotracker.h"
#include "getBackground.h"
#include "extractBlob.h"
#include "kalman.h"



///Functions Declaration

void execute(char * ,int );

void drawBlob (IplImage *, struct coordinate , int, int , int );

#endif

