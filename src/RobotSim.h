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
  float MAXturningSpeed;
  int searchRadius;
  double turningStack;
  bool cantakenewAngle;


public:
  // Vars
  cv::Point2d Position;
  float angle;

  // constructor
  // TODO make a click event for the starting point
  RobotSim (Point2d Position, float angle, String name = "", float turningSpeed = 2.0, Size R_size = Size(60,35));

  // Functions
  void set_searchRadius(int searchRadius);
  void draw(Mat& src, bool draw_searchRadius = false);
  void drawTrace(Mat& src);
  void showDirection(Mat& src, float value1, float value2);
  void move(float dist, float angle);
  void drive(float velocity, float angle, float turningSpeed = 0.5);
};

#endif /* ROBOTSIM_H_ */
