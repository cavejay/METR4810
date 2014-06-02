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

#include "preProc.h"
#include "C++/MinGW/RR_API.h"

using namespace cv;
using namespace std;

enum ED_SHAPE {
  ED_RECTANGLE = 0,
  ED_CROSS = 1,
  ED_ELLIPSE = 2
};

// Functions
void Draw_Circles(Mat& img, const vector<Vec3f>& circles);
void grabClickPoint(int event, int x, int y, int flags, void *ptr);
void grabClickPointVector(int event, int x, int y, int flags, void *ptr);
void mouseHandler_pit(int event, int x,int y, int flags, void *ptr);
void Dilation(const Mat& src, Mat& dst, ED_SHAPE dilation_shape, double dilation_size);
void Erosion(const Mat& src, Mat& dst, ED_SHAPE erosion_shape, double erosion_size);
bool less_vectors(const std::vector<Point>& vec1, const std::vector<Point>& vec2);
string int2str(int number);

int getSearchRadius(int preferedPoints, int sideMinimum, Point2f carCenter, vector<Point> outside, vector<Point> inside);
vector<Point2f> findCorner(Point xy);
#endif /* FUNCTIONS_H_ */
