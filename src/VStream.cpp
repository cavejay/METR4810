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
VStream::VStream (inputVars data)
{
  this->Host = data.Host;
  this->file_loc = data.file_location;
  this->cameraNumber = data.camera_number;
  _inputFormat = hashit(data.inputSource);
  switch(_inputFormat)
    {
      case VIDEO_FILE:
	  video();
        break;

      case VIDEO_CAMERA:
	  camera();
        break;

      case ROBOREALM:
	  roborealm();
        break;

      case STILL_IMAGE:
	  still();
        break;

      default:
        cout << "Invalid input source was provided." << endl;
        show_usage("METR4810.exe");
        break;
    }
}

VStream::~VStream ()
{
  // TODO Auto-generated destructor stub
}

/**
 * Creates a Video stream from the path given in the cmd line
 */
int VStream::video()
{
  cap.open(file_loc); // Load Video file
  if ( !cap.isOpened() )  // if not success, exit program
  {
    cout << "Cannot open the video file" << endl;
    return -1;
  } else {
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    cout << "Frame per seconds: " << fps << endl; // Show frames
    return 1;
  }
}

/**
 * Initialises a connection to a roborealm API server and grabs the width and height of its camera
 */
int VStream::roborealm()
{
  int* p_width = &roboWidth;
  int* p_height = &roboHeight;
  rr.connect("localhost",6060);
  cout << ".......connected\n";
  bool success = rr.getDimension(p_width, p_height);
  rr.disconnect();
  cout << "disconnected\n";
  cout << "width: " << roboWidth << "\nheight " << roboHeight << endl;
  return success;
}

/**
 * Setups up to load a still image
 */
int VStream::still()
{
  // There is absolutely nothing to do here...
  return true;
}

/**
 * Creates a Video stream from the camera at the number provided to the cmd line
 */
int VStream::camera()
{
  cap.open(cameraNumber); // Load Video camera #0
  if (!cap.isOpened())  // if not success, exit program
  {
    cout << "Cannot open camera '0' closing program" << endl;
    return -1;
  }
  else {
    vidSize.width = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    vidSize.height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    // Show what you're reporting.
    cout << "Frame size : " << vidSize.width << " x " << vidSize.height << endl;
    return 1;
  }
}

/**
 * Gets an image from the stream specified by CurrentlyUsing
 * and outputs it as a Mat.
 */
Mat VStream::pullImage(int port)
{ //
  bool readCorrectly;
  Mat dst;
  switch(_inputFormat)
  {
    case VIDEO_FILE:
      cout << "pulling from video file?" << endl;
      readCorrectly = cap.read(dst);
      if(!readCorrectly){cerr << "The frame could not be read" << endl;}
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

inputFormat VStream::hashit(const String inString){
  if(inString == "still") return STILL_IMAGE;
  if(inString == "camera") return VIDEO_CAMERA;
  if(inString == "video") return VIDEO_FILE;
  if(inString == "roborealm") return ROBOREALM;
  else {return BADNESS;}
}

Mat VStream::roboGrab(char* host, int port){
  // Reserve pixel space
  uchar *data = new uchar[roboWidth * roboHeight * 3];

  // Picture to return
  Mat img_out;

  // Connect and grab stuff.
  cout << "running getImage()\n";

  bool connected = rr.connect("localhost", port);
  if(!connected){
	  cout << "ERROR: Could not connect to Roborealm." << endl;
	  return img_out;
  }

  // Wait for latest image
  rr.waitImage();

  // receive the image
  bool imgReceived = rr.getImage(data, &roboWidth, &roboHeight, roboWidth * roboHeight * 3);
  if (!imgReceived){
	cout << "API call failed. Perhaps due to a timeout?" << endl;
  }

  rr.disconnect();
  cout << "Disconnected\n";
  cout << "Showing pixel data: \n" << data << endl;

  // Make a Mat from the pixel data
  cv::Mat img(roboHeight, roboWidth, CV_8UC3, data);
  cv::cvtColor(img, img_out, CV_RGB2BGR);
  delete [] data;

  // If the frame isn't empty show it.
  if (!img_out.empty())
  {
    cout << "Frame has data, and is being shown" << endl;
    cv::imshow("Grabbed Image", img_out);
  } else {
    cout << "Frame is empty" << endl;
  }
  return img_out;
}

