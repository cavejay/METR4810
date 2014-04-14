/*
 * RobotSim.h
 *
 *  Created on: 11/04/2014
 *      Author: Michael
 */

#ifndef ROBOTSIM_H_
#define ROBOTSIM_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#define PI_NUM 3.14159265

using namespace cv;
using namespace std;

class RobotSim
{
private:
  // Internal
  vector<Point2d> History;
  Size R_size;
  String name;
  Mat repr;

public:
  // Vars
  cv::Point2d Position;
  float angle;

  // constructor
  RobotSim (Point2d Position, float angle, String name = "", Size R_size = Size(60,35), String Repr = "Sample_Pictures/mach5.png");

  // Functions
  void draw(Mat& src, int search_circle = 0);
  void move(int dist, float angle);
};

#endif /* ROBOTSIM_H_ */
