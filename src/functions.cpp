/*
 * functions.cpp
 *
 *  Created on: 28/03/2014
 *      Author: Michael
 */
#include "functions.h"

int FindInput(char* argv0)
{
  int CurrentlyUsing;
  if(*argv0 == 0){
    CurrentlyUsing = VIDEO_FILE;

  } else if (*argv0 == 1){
    CurrentlyUsing = VIDEO_CAMERA;

  } else if (*argv0 == 2){
    CurrentlyUsing = ROBOREALM;

  } else {
    cout << "No Valid Video Input was specified. Proceding with a still image" << endl;
    CurrentlyUsing = STILL_IMAGE;
  }
  return CurrentlyUsing;
}

int init_videocapture(int video_source, VideoCapture& cap, const string& file_loc)
{
  switch (video_source) // Do different things for different video sources :)
  {
  case VIDEO_FILE: // The source is a video file.
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

  case VIDEO_CAMERA:
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
  default: // Use the RR_API to make magic happen
    cout << "No case was called for init_videocapture.\nClosing Program" << endl;
    return -1;
    break;
  }
  return 0;
}

int init_videocapture(int video_source, RR_API& rr, char* ServerAddress)
{
  if(video_source == ROBOREALM)
  {
//    strcpy(Host,ServerAddress.c_str());
    rr.connect(ServerAddress,6060);
  } else {
    cout << "The 2nd variable must be a videoCapture object to run from a file or video camera.\nClosing Program" << endl;
    return -1;
  }
  return 0;
}

Mat pullImage(int CurrentlyUsing, RR_API& rr, VideoCapture& cap)
{ //
  Mat dst;
  switch(CurrentlyUsing)
  {
    case VIDEO_FILE:
      cap >> dst;
      return dst;
      break;

    case VIDEO_CAMERA:
      cap >> dst;
      return dst;
      break;

    case ROBOREALM:

      return dst;
      break;

    default:
      cout << "No source was specified. Image could not be pulled.\n Empty Mat is supplied" << endl;
      return dst;
      break;
  }
  return dst;
}


void Draw_Circles(Mat& img, const vector<Vec3f>& circles)
{
  cout << "Draw " << circles.size() << " of dem circles" << endl;

  for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      cout << radius <<endl;
      // draw the circle center
      circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // draw the circle outline
      circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
}


void Dilation(const Mat& src, Mat& dst, int dilation_shape, double dilation_size)
{
  int dilation_type;
  if( dilation_shape == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_shape == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_shape == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  cv::dilate( src, dst, element);
}


void Erosion(const Mat& src, Mat& dst, int erosion_shape, double erosion_size)
{
  int erosion_type;
  if( erosion_shape == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_shape == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_shape == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
  /// Apply the erosion operation
  erode( src, dst, element);
}















