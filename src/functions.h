/*
 * functions.h
 *
 *  Created on: 28/03/2014
 *      Author: Michael
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

enum {
VIDEO_CAMERA = 1,
VIDEO_FILE = 0
};

// Functions
int init_videocapture(VideoCapture& cap, int video_source, const string& file_loc = "C:/Sample.avi");


#endif /* FUNCTIONS_H_ */
