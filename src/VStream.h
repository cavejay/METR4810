/*
 * VStream.h
 *
 *  Created on: 03/04/2014
 *      Author: Michael
 */

#ifndef VSTREAM_H_
#define VSTREAM_H_

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

#include "functions.h"
#include "startup.h"
// Namespaces
using namespace cv;
using namespace std;

enum inputFormat{
  VIDEO_FILE = 0,
  VIDEO_CAMERA = 1,
  ROBOREALM = 2,
  STILL_IMAGE = 3,
  BADNESS = 100
};

class VStream
{
private:
  // Varibles
  int roboHeight;
  int roboWidth;
  int ports[4] = {6060,6061,6062,6063};
  int cameraNumber;
  Size vidSize;

  int init_videocapture(int video_source, VideoCapture& cap, const string& file_loc);
  Mat roboGrab(char* host, int port);
  inputFormat hashit(const String inString);

  // Types of inits
  int roborealm();
  int still();
  int video();
  int camera();


public:
  cv::VideoCapture cap;
  RR_API rr;
  int _inputFormat;
  char* Host;
  string file_loc;

  // Constructor
  VStream (inputVars data);

  // Functions
  cv::Mat pullImage(int port = 0);


  virtual
  ~VStream ();
};

#endif /* VSTREAM_H_ */
