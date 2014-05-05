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
// Namespaces
using namespace cv;
using namespace std;

class VStream
{
private:
  // Varibles
  int roboHeight;
  int roboWidth;
  int ports[4] = {6060,6061,6062,6063};

  int init_videocapture(int video_source, VideoCapture& cap, const string& file_loc);
  int init_videocapture(int video_source, RR_API& rr, char* ServerAddress);
  Mat roboGrab(char* host, int port);

public:
  cv::VideoCapture cap;
  RR_API rr;
  int CurrentlyUsing;
  char* Host;
  string file_loc;


  // Constructor
  VStream (int CurrentlyUsing, char* Host = "127.0.0.1", string file_loc = "C:/Sample.avi");

  // Functions
  int FindInput();
  int StartInput();
  cv::Mat pullImage(int port = 0);


  virtual
  ~VStream ();
};

#endif /* VSTREAM_H_ */
