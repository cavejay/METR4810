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
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
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
  CarLocator (inputVars in);
  void findCar_SURF(const Mat& src);
  Point findCar(const Mat& src);

};

#endif /* LOCATOR_H_ */
