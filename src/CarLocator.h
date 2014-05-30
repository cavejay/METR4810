/*
 * Locator.h
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */

#ifndef CARLOCATOR_H_
#define CARLOCATOR_H_

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
#include <vector>
#include <deque>

#include "functions.h"
#include "startup.h"

class CarLocator
{
private:
  vector<Point> History;
  deque<Mat> imgHistory;

  void newIMG(const Mat& img);
  Mat getWAvg(void);

public:
  CarLocator ();
  Point findCar(const Mat& src);

};

#endif /* LOCATOR_H_ */
