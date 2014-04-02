// METR4810 Off-Board Software
// version: 0.01a

// OpenCV
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"

// ARuCo
#include "src/aruco.h"
#include "src/cvdrawingutils.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "C++/MinGW/RR_API.h"

using namespace cv;
using namespace std;
using namespace aruco;

static void help()
{ // This is here for when this becomes a command-line program :)
  cout << "This program is currently not meant to run from the command line.\n"
      "Please read the source and either recompile with the right settings." << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 2){help();}


  int CurrentlyUsing = FindInput(argv[0]);
  VideoCapture cap;
  RR_API rr;

  // initialise the appropriate video device. This is kinda messy but needed because I need it.
  int vidcap_result;
  if(CurrentlyUsing == ROBOREALM)
  {
    cout << "Using RoboRealm for Image aquisition" << endl;
    char* host;
    if(argv[1]){host = argv[1];} else {host = "127.0.0.1";} // If we're told where to connect to, do that. else connect to this computer
    vidcap_result = init_videocapture(CurrentlyUsing,rr, host); // Initialise the magic rr system thingo
  } else

    if (CurrentlyUsing == STILL_IMAGE)
  {
    cout << "Using a still image for Image aquisition" << endl;
  } else if (CurrentlyUsing == VIDEO_CAMERA || CurrentlyUsing == VIDEO_FILE){
    cout << "Using OpenCV's Video Capture method for Image aquisition" << endl;
    string file_location = argv[1];
    vidcap_result = init_videocapture(CurrentlyUsing,cap,file_location);
  } else {
    cout << "CurrentlyUsing was an unexpected value. \nClosing program" << endl;
    return -1;
  }

  // If no video or anything was loaded correctly crash.
  if (vidcap_result == -1) {return -1;}
  else {
	cout << "Video capture has been properly defined and started" << endl;
  }


  Mat img; ///== imread("img.jpg");

  // Start Loop
  for(;;)
  {
    // Grab current image from which ever source has been specificied.
    img = pullImage(CurrentlyUsing, rr, cap);

  // Initialize Variables
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThresTrack;
  if(!img.empty())
  {
    cv::imshow("rawr",img);
    cout << "I got it yo\n";
  } else {
    cout << "Yo, the image is empty???\n";
    return -1;
  }

  frame_bgr = img;

  // Make le windows
//  cv::namedWindow("Basic Stream",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  /*  This is where we put all the looping stuff.
   *  I'm thinking only a single thread atm.
   *  The for(;;) also will go just above here :) */

    // Gray
    cv::cvtColor( frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::imshow( "Example Gray", frame_gry );

    // Canny
    cv::Canny( frame_gry, frame_cny, 10, 100, 3, true );
    cv::imshow("Example Canny", frame_cny);

    // Track as black
    cv::namedWindow("Thresholded Image", CV_WINDOW_AUTOSIZE);
    int threshMag = 0;
    int d_val;
    int e_val;
    int two50 = 255;
    int fifty = 50;
    cv::cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);
    vector<Mat> hsvchannels(frame_hsv.channels());
    cout << frame_hsv.channels() << endl;
    cv::split(frame_hsv, hsvchannels);
    cv::createTrackbar("threshold value", "Thresholded Image", &threshMag,two50, NULL);
    cv::createTrackbar( "Dilation size", "Thresholded Image", &d_val, fifty, NULL);
    cv::createTrackbar( "Erosion size", "Thresholded Image", &e_val,fifty, NULL);

    cv::imshow("H", hsvchannels[0]);
    cv::imshow("V", hsvchannels[2]);
    cv::threshold(hsvchannels[2], ThresTrack,150,255,cv::THRESH_BINARY);
    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,0.5);
    Erosion(ThresTrack,ThresTrack,ED_RECTANGLE,0.3);
    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,0.8);
    cv::imshow("Threshold Test", ThresTrack);

    //Circle
    cv::cvtColor(frame_bgr, frame_gry, CV_BGR2GRAY);
    // smooth it, otherwise a lot of false circles may be detected
    cv::GaussianBlur( frame_gry, frame_gry, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    cv::HoughCircles(frame_gry, circles, CV_HOUGH_GRADIENT, 2, 10, 200, 100, 0,200);
    Draw_Circles(frame_bgr,circles);
    cv::imshow( "circles", frame_bgr);

    try
    {
        aruco::MarkerDetector MDetector;
        vector<Marker> Markers;
        //read the input image
        cv::Mat InImage;
        cap >> InImage;
     //Ok, let's detect
        MDetector.detect(InImage,Markers);
        //for each marker, draw info and its boundaries in the image
        for (unsigned int i=0;i<Markers.size();i++) {
            cout<<Markers[i]<<endl;
            Markers[i].draw(InImage,Scalar(0,0,255),2);
        }
        cv::imshow("in",InImage);
//        cv::waitKey(0);//wait for key to be pressed
    } catch (std::exception &ex)
    {
        cout<<"aruco failed\nException :"<<ex.what()<<endl;
    }


//    Don't need this yet :)
    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
//  waitKey();
//  return 0;

}
