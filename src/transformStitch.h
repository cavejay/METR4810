/*
 * transformStitch.h
 *
 *  Created on: 28/05/2014
 *      Author: Michael
 */

#ifndef TRANSFORMSTITCH_H_
#define TRANSFORMSTITCH_H_

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

class camTrans {
	// stores each camera's transformation matrices in as <port, transform matrice>
	map<int, Mat> cameraTransforms;
	map<int, Mat> cameraIMGs;
	vector<int> ports;

public:
	camTrans(VStream Vs);
	void stitch(Mat& src1, Mat& src2, Mat& dst);
	Mat cameraTransform(int Port);
};

#endif /* TRANSFORMSTITCH_H_ */
