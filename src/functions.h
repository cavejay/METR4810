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

// Structs
struct inputVars {
  bool varsParsedSuccessfully = true;
  String inputSource = "still";
  char* Host = "127.0.0.1";
  String file_location = "Sample_Pictures/demo-track.png";
  int camera_number = 0;
  bool multithreading = false;
  bool Simulation = false;
  bool demo_mode = false;
};

// Functions
inputVars getInputData(int argc, char* argv[]);

void Draw_Circles(Mat& img, const vector<Vec3f>& circles);
void Dilation(const Mat& src, Mat& dst, int dilation_shape, double dilation_size);
void Erosion(const Mat& src, Mat& dst, int erosion_shape, double erosion_size);
bool less_vectors(const std::vector<Point>& vec1, const std::vector<Point>& vec2);

int getSearchRadius(int preferedPoints, Point2f carCenter, vector<Point> outside, vector<Point> inside);
#endif /* FUNCTIONS_H_ */
