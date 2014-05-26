/*
 * race_track_extraction.h
 *
 *  Created on: 2014Äê5ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef RACE_TRACK_EXTRACTION_H_
#define RACE_TRACK_EXTRACTION_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"
// C libs
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace cv;
using namespace std;

void removeSmallBlobs(Mat image, double size);

Mat race_track_extraction(Mat image,double top_const,
		double bot_const,double left_const, double right_const);




#endif /* RACE_TRACK_EXTRACTION_H_ */
