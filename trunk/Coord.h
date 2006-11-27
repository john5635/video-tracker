#ifndef _COORD_H
#define _COORD_H

/*! \file Coord.h
 *
 *
 *  \brief <b>This file contains the function that aim to catch the blob in the video using the cvBlob library using opencv. The taken blob from the pool is that provied by the ID passed throught the function </b>
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
 *
 *
 */


class coord{
public:
	int MinX;
	int MaxX;
	int MinY;
	int MaxY;
	double cX;
	double cY;
	int lX;
	int lY;
	bool flag;

	coord(){
		MinX=0;
		MaxX=0;
		MinY=0;
		MaxY=0;
		cX=0;
		cY=0;
		lX=0;
		lY=0;
		flag=true;
	}

	coord(int x,int y){
		MinX=0;
		MaxX=0;
		MinY=0;
		MaxY=0;
		cX=x;
		cY=y;
		lX=0;
		lY=0;
		flag=true;
	}

	coord(int maxx,int minx, int maxy, int miny){
		MinX=minx;
		MaxX=maxy;
		MinY=miny;
		MaxY=maxy;
		cX=(MinX+MaxX)/2;
		cY=(MinY+MaxY)/2;
		lX=MaxX-MinX;
		lY=MaxY-MinY;
		flag=true;
	}

	void set(int x,int y){
		cX=x;
		cY=y;
	}

	void set(int maxx,int minx, int maxy, int miny){
		MinX=minx;
		MaxX=maxx;
		MinY=miny;
		MaxY=maxy;
		cX=(MinX+MaxX)/2;
		cY=(MinY+MaxY)/2;
		lX=MaxX-MinX;
		lY=MaxY-MinY;
	}

	//~coord();//distruttore
};

#endif
