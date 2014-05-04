/*
 * VStream.cpp
 *
 *  Created on: 03/04/2014
 *      Author: Michael
 */

#include "VStream.h"

/**
 * The Constructor for VStream takes the video_source, the roborealm host and a file location.
 * Both the host ip and the file loc aren't required.
 */
VStream::VStream (int iCurrentlyUsing, char* iHost, string ifile_loc)
{
  CurrentlyUsing = iCurrentlyUsing;
  Host = iHost;
  file_loc = ifile_loc;

//  args = argv;
}

VStream::~VStream ()
{
  // TODO Auto-generated destructor stub
}

/**
 * 			DEAD
 *
 * From the argv it is determined which form of input is requested.
 * This sets the internal variable CurrentlyUsing appropriately.
 */
//int VStream::FindInput()
//{
//  if(strcmp(args[1],"0")){
//    cout << "args: " << args[1] << "0: " << "0" << endl;
//    CurrentlyUsing = VIDEO_FILE;
//
//  } else if (strcmp(args[1],"1")){
//    CurrentlyUsing = VIDEO_CAMERA;
//
//  } else if (strcmp(args[1], "2")){
//    CurrentlyUsing = ROBOREALM;
//
//  } else {
//    cout << "No Valid Video Input was specified. Proceding with a still image" << endl;
//    CurrentlyUsing = STILL_IMAGE;
//  }
//  return CurrentlyUsing;
//}

/**
 * Starts the stream specified by CurrentlyUsing.
 * Calls the help function init_videocapture to help it with this.
 */
int VStream::StartInput()
{
  int vidcap_result;
  if(CurrentlyUsing == ROBOREALM)
  {
    cout << "Using RoboRealm for Image aquisition" << endl;
    vidcap_result = init_videocapture(CurrentlyUsing,rr,Host); // Initialise the magic rr system thingo.
    cout << "video init passed\n";
    return 1;

  } else
  if (CurrentlyUsing == STILL_IMAGE)
  {
    cout << "Using a still image for Image aquisition" << endl;
    return 1;

  } else
  if (CurrentlyUsing == VIDEO_CAMERA || CurrentlyUsing == VIDEO_FILE){
    cout << "Using OpenCV's Video Capture method for Image aquisition" << endl;
    vidcap_result = init_videocapture(CurrentlyUsing,cap,file_loc);
  }

  else {
   cout << "CurrentlyUsing was an unexpected value. \nClosing program" << endl;
   return -1;
  }

  // If no video or anything was loaded correctly, crash.
  if (vidcap_result == -1) {cout << "video failed to load" << endl;return -1;}
  else {
	cout << "Video capture has been properly defined and started" << endl;
	return 1;
  }
}

/**
 * Gets an image from the stream specified by CurrentlyUsing
 * and outputs it as a Mat.
 */
Mat VStream::pullImage(int port)
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
      return roboGrab(Host, port);
      break;

    case STILL_IMAGE:
      dst = imread(file_loc);
      return dst;
      break;

    default:
      cout << "No source was specified. Image could not be pulled.\n Empty Mat is supplied" << endl;
      return dst;
      break;
  }
  return dst;
}

/**
 * Private helper class that initialises the input from though the opencv
 * VideoCapture object.
 */
int VStream::init_videocapture(int video_source, VideoCapture& cap, const string& file_loc)
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

/**
 * Private helper class that initialises the input thought the RoboRealm API. yuk.
 * This isn't finished yet, merely opening a connect at this stage.
 */
int VStream::init_videocapture(int video_source, RR_API& rr, char* ServerAddress)
{
  if(video_source == ROBOREALM)
  {
    int* p_width = &roboWidth;
    int* p_height = &roboHeight;
    cout << "connecting\n";
    rr.connect(Host,6060);
    cout << ".......connected\n";
    rr.getDimension(p_width, p_height);
    rr.disconnect();
    cout << "disconnected\n";
    cout << "width: " << roboWidth << "\nheight " << roboHeight << endl;

  } else {
    cout << "The 2nd variable must be a videoCapture object to run from a file or video camera.\nClosing Program" << endl;
    return -1;
  }
  return 0;
}

Mat VStream::roboGrab(char* host, int port){
  // Reserve pixel space
  unsigned char* pixels = (unsigned char *)malloc(roboWidth*roboHeight*1);

  // Connect and grab stuff.
  cout << "running getImage()\n";
  rr.connect(host, port);
  rr.getImage("",pixels,&roboWidth, &roboHeight,roboWidth*roboHeight,"GRAY");
  rr.disconnect();
  cout << "got pixel data\n";

  // Make a Mat from the pixel data
  Size imgSize = Size(roboHeight,roboWidth);
  cv::Mat frame = cv::Mat(imgSize, CV_8UC1, pixels);

  if (!frame.empty())
  {
    cout << "Frame has data, and is being shown" << endl;
    cv::imshow("Grabbed Image", frame);
  } else {
    cout << "Frame is empty" << endl;
  }

  // Prove you got pixels
//  cout << "pixels: " << pixels << endl << "p_pixels: " << &pixels << endl;

  // Free pixel space
  free(pixels);
  return frame;
}

