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

static void help()
{ // This is here for when this becomes a command-line program :)
  cout << "This program is currently not meant to run from the command line.\n"
      "Please read the source and either recompile with the right settings." << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 3){help();cin.get();exit(0);}

  VStream Vs(argv);
  Vs.FindInput();
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
      cv::imshow("rawr",img);
      cout << "I got it yo\n";
    } else {
      cout << "Yo, the image is empty???\n";
      return -1;
    }

    frame_bgr = img.clone();

//  cv::namedWindow("Basic Stream",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    try
    {
      // Gray
      cv::cvtColor( frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
      cv::imshow( "Example Gray", frame_gry );
      cout << "rawr1\n";
      // Canny
      cv::Canny( frame_gry, frame_cny, 10, 100, 3, true );
      cv::imshow("Example Canny", frame_cny);

      // Track as Black

      cv::cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);
      vector<Mat> hsvchannels(3);
      cout << frame_hsv.channels() << endl;
      cv::split(frame_hsv, hsvchannels);
//      cv::imshow("H", hsvchannels[0]);
//      cv::imshow("V", hsvchannels[2]);
      cv::threshold(hsvchannels[2], ThresTrack,150,255,cv::THRESH_BINARY);
      Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,d_val);
      Erosion(ThresTrack,ThresTrack,ED_RECTANGLE,e_val);
      Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,d_val);
      cv::imshow("Threshold Test", ThresTrack);

      //Circle
      cv::cvtColor(frame_bgr, frame_gry, CV_BGR2GRAY);
      // smooth it, otherwise a lot of false circles may be detected
      cv::GaussianBlur( frame_gry, frame_gry, Size(9, 9), 2, 2 );
      vector<Vec3f> circles;
      cv::HoughCircles(frame_gry, circles, CV_HOUGH_GRADIENT, 2, 10, 200, 100, 0, 200);
      Draw_Circles(frame_bgr,circles);
      cv::imshow( "circles", frame_bgr);
    } catch (std::exception &ex)
    {
      cout<<"Improc failed\nException :"<<ex.what()<<endl;
    }


    try
    {
      aruco::MarkerDetector MDetector;
      vector<Marker> Markers;
      //read the input image
      cv::Mat InImage = Vs.pullImage();
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

    /* Beginning of section for rotation */



    // Run the elements from rotate3d and show them
    try
    {
      Mat src = imread("Sample_Pictures/S5.jpg");
      Mat warp_dst, warp_rotate_dst;

      cv::Mat* frameArray = new cv::Mat[2];

      frameArray = Rotate(src);
      warp_dst = frameArray[0];
      warp_rotate_dst = frameArray[1];
      namedWindow("Source image", CV_WINDOW_AUTOSIZE );
      imshow("Source image", src );

      namedWindow("Warp", CV_WINDOW_AUTOSIZE );
      imshow("Warp", warp_dst );

      namedWindow("Warp + Rotate", CV_WINDOW_AUTOSIZE );
      imshow("Warp + Rotate", warp_rotate_dst );
    } catch (std::exception &ex) {
      cout<<"rotation failed\nException :"<<ex.what()<<endl;
    }

    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
//  waitKey();
//  return 0;

}
