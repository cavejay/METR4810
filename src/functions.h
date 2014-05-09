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

#include "C++/MinGW/RR_API.h"

using namespace cv;
using namespace std;

enum {
  ED_RECTANGLE = 0,
  ED_CROSS = 1,
  ED_ELLIPSE = 2
};

// Functions
void Draw_Circles(Mat& img, const vector<Vec3f>& circles);
void Dilation(const Mat& src, Mat& dst, int dilation_shape, double dilation_size);
void Erosion(const Mat& src, Mat& dst, int erosion_shape, double erosion_size);
bool less_vectors(const std::vector<Point>& vec1, const std::vector<Point>& vec2);
#endif /* FUNCTIONS_H_ */
