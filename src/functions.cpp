/*
 * functions.cpp
 *
 *  Created on: 28/03/2014
 *      Author: Michael
 */
#include "functions.h"

int init_videocapture(VideoCapture& cap, int video_source, const string& file_loc)
{
  switch (video_source)
  {
  case 0: // The source is a video file.
    cap.open(file_loc); // Load Video file
    if ( !cap.isOpened() )  // if not success, exit program
    {
      cout << "Cannot open the video file" << endl;
      return -1;
    } else {
      double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
      cout << "Frame per seconds : " << fps << endl; // Show frames
      return 1;
    }
    break;
  case 1:
   cap.open(0); // Load Video camera #0
    if (!cap.isOpened())  // if not success, exit program
    {
      cout << "Cannot open camera '0' closing program" << endl;
      return -1;
    }
    else {
      double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
      double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
      // Show what you're reporting.
      cout << "Frame size : " << dWidth << " x " << dHeight << endl;
      return 1;
    }
  }
  return 0;
}


