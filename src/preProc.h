/*
 * preProc.h
 *
 *  Created on: 28/05/2014
 *      Author: Michael
 */

#ifndef PREPROC_H_
#define PREPROC_H_

// OpenCV
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"
// RobotRealm Lib
#include "C++/MinGW/RR_API.h"
// C libs
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>

#include "functions.h"
#include "startup.h"
#include "VStream.h"

// Namespaces
using namespace cv;
using namespace std;

class preProc {
public:
	// Variables
	// Temporary variable to handle click points
	vector<Point2f> transformPoints;

	// The 2 largest contours used for pathing
	vector<vector<Point> > trackContours;


	// Functions/Methods
	preProc();
//	preProc(VStream Vs, inputVars in);
	void overlay(Mat& src1, Mat& src2, Mat& dst);

};

#endif /* PREPROC_H_ */
