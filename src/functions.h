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
#include <vector>

#include "M:/BitBucket/METR4810/Roborealm_API/C++/MinGW/RR_API.h"

using namespace cv;
using namespace std;

enum {
  VIDEO_FILE = 0,
  VIDEO_CAMERA = 1,
  ROBOREALM = 2,

  ED_RECTANGLE = 0,
  ED_CROSS = 1,
  ED_ELLIPSE = 2
};

// Functions
int init_videocapture(int video_source, VideoCapture& cap, const string& file_loc = "C:/Sample.avi");
int init_videocapture(int video_source, RR_API& rr, const string& Network_Address);
void Draw_Circles(Mat& img, const vector<Vec3f>& circles);
void Dilation(const Mat& src, Mat& dst, int dilation_shape, double dilation_size);
void Erosion(const Mat& src, Mat& dst, int erosion_shape, double erosion_size);
#endif /* FUNCTIONS_H_ */
