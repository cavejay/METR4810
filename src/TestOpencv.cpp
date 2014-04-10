// METR4810 Off-Board Software
// version: 0.01a

// OpenCV
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"
// ARuCo
#include "src/aruco.h"
#include "src/cvdrawingutils.h"
// RobotRealm Lib
#include "C++/MinGW/RR_API.h"
// C libs
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Our .h's
#include "functions.h"
#include "Rotate3d.h"
#include "VStream.h"
// Definitions
#define ever ;;
// Namespaces
using namespace cv;
using namespace std;
using namespace aruco;

// Screw cmdline stuff for now. That gave me a headache >:(

//static void help()
//{ // This is here for when this becomes a command-line program :)
//  std::cout << "Usage is -s <video_source>\n	Additional commands are: \n	-h <host>\n	"
//                "-loc <file_location>\n	-c <Camera_Number>"; // Inform the user of how to use the program
//  std::cin.get();
//  exit(0);
//}

int main(void) // int argc, char* argv[]
{
//  if (argc < 2) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
//    help();
//  }// if we've got enough parameters...

  // Srsly thou. Just stick want you want in this before you compile. Screw cmdline for now :(

  VStream Vs(STILL_IMAGE, "127.0.0.1", "Sample_Pictures/track-example3.png");
//  Vs.FindInput();
  Vs.StartInput();


  // initialise the appropriate video device. This is kinda messy but needed because I need it lulz and modularity


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

  // Initialise all dem variables

  cv::Mat img, img1; ///== imread("img.jpg");
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThresTrack;
  int threshMag = 0;
  int d_val, e_val;
  int two50 = 255;
  int fifty = 50;


  cv::namedWindow("Thresholded Image", CV_WINDOW_AUTOSIZE);
  cv::createTrackbar("threshold value", "Thresholded Image", &threshMag,two50, NULL);
  cv::createTrackbar( "Dilation size", "Thresholded Image", &d_val, fifty, NULL);
  cv::createTrackbar( "Erosion size", "Thresholded Image", &e_val,fifty, NULL);
  // Start Loop
  for(ever)
  {
    // Grab current image from which ever source has been specified.
    img = Vs.pullImage();

    // Check if the image is empty. There's not point continuing if so.
    if(!img.empty())
    {
      cv::imshow("Grabbed Image",img);
      cout << "I got it yo\n";
    } else {
      cout << "Yo, the image is empty???\n";
      return -1;
    }

    frame_bgr = img.clone();

    cv::cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);
    vector<Mat> hsvchannels(3);
    cout << frame_hsv.channels() << endl;
    cv::split(frame_hsv, hsvchannels);
//      cv::imshow("H", hsvchannels[0]);
      cv::imshow("V", hsvchannels[2]);
//    cv::threshold(hsvchannels[2], ThresTrack,150,255,cv::THRESH_BINARY);
//    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,d_val);
//    Erosion(ThresTrack,ThresTrack,ED_RECTANGLE,e_val);
//    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,d_val);
//    cv::imshow("Threshold Test", ThresTrack);




    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
//  waitKey();
//  return 0;

}
