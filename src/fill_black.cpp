/*
 * fill_black.cpp
 *
 *  Created on: 2014Äê5ÔÂ28ÈÕ
 *      Author: Administrator
 */
#include "fill_black.h"

Mat fill_black(Mat image,Point p)
{
	floodFill(image, p, 0,0,Scalar(),Scalar(),4);
	return image;
}



