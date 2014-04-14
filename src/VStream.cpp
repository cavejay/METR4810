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
Mat VStream::pullImage()
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
    int width;
    int* p_width = &width;
    int height;
    int* p_height = &height;
    rr.getDimension(p_width, p_height);
    unsigned char* pixels = (unsigned char *)malloc(width*height*1);
    unsigned char* pixels2 = (unsigned char *)malloc(width*height*3);
//    unsigned char* p_pixels = &pixels;
    cout << "made needed vars and pointers\n";

    cout << "connecting\n";
    rr.connect(ServerAddress,6060);
    cout << ".......connected\n\nrunning getImage()\n";

    rr.getImage("",pixels,p_width, p_height,width*height,"GRAY");
    cout << "got Image\n";

    Mat rawr = cv::imdecode(*pixels, CV_LOAD_IMAGE_GRAYSCALE);
    if (!rawr.empty()){imshow("haah!",rawr);} else {cout << "picture was empty\n";}
    cout << "decoded and shown\n";

//    rr.getBitmap("processed", pixels2, p_width, p_height, width*height*3);
//    cout << "bitmap got\n";
//    Mat rawr2 = cv::imdecode(*pixels2, CV_LOAD_IMAGE_COLOR);
//    if (!rawr2.empty()){imshow("haah!2",rawr2);} else {cout << "picture2 was empty\n";}
//    cout << "decoded pixels2 and shown\n";

    rr.disconnect();
    cout << "disconnected\n";
    cout << "width: " << width << "\nheight " << height << endl;
    cout << "pixels: " << pixels << endl << "p_pixels: " << &pixels << endl;;
    free(pixels);
    free(pixels2);



  } else {
    cout << "The 2nd variable must be a videoCapture object to run from a file or video camera.\nClosing Program" << endl;
    return -1;
  }
  return 0;
}
