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

#include "functions.h"
// Namespaces
using namespace cv;
using namespace std;

class VStream
{
private:
  int init_videocapture(int video_source, VideoCapture& cap, const string& file_loc);
  int init_videocapture(int video_source, RR_API& rr, char* ServerAddress);

public:
  cv::VideoCapture cap;
  RR_API rr;
  int CurrentlyUsing;
  char* args[];


  // Constructor
  VStream (char* argv[]);

  // Functions
  int FindInput();
  int StartInput();
  cv::Mat pullImage();


  virtual
  ~VStream ();
};

#endif /* VSTREAM_H_ */
