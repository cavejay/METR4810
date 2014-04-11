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
<<<<<<< Updated upstream
// Aruco .h's
#include "src/marker.h"
#include "src/aruco.h"
#include "src/cvdrawingutils.h"
=======
#include "RobotSim.h"
>>>>>>> Stashed changes
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

  VStream Vs(STILL_IMAGE, "127.0.0.1", "Sample_Pictures/track-example2.png");
//  Vs.FindInput();
//  initialise the appropriate video device. This is kinda messy but needed because I need it 4lulz and modularity
  Vs.StartInput();


  // Make Robot SImulator
  RobotSim Rsim = RobotSim(Point2d(400,250),0,"Robot1");

  VideoWriter Vw("rawr.avi",-1,30,Size(838,670));


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


  // Initialise all dem variable
  cv::Mat img, img1; ///== imread("img.jpg");
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
  int threshMag = 0;

  vector<int> compression_params;
      compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      compression_params.push_back(9);

  // Start Loop
  int count = 0;
  String filename = "img";
  for(ever)
  {
    count++;
    /*
     * 		Pull Image and init
     *
     */
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

    /*
     * 		Grab contours draw them
     *
     *
     */

    cv::cvtColor(frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::threshold( frame_gry, ThreshTrack, threshMag, 255, THRESH_BINARY );

    // Contours are a vector of vectors of points
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::findContours( ThreshTrack, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cv::Mat drawing = cv::Mat::zeros( ThreshTrack.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
      Scalar color = Scalar(0,255,100);
      drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }

     /// Show in a window
     namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
     Rsim.move(3, 1.0);
     Rsim.draw(drawing);
     imshow( "Contours", drawing );

//     Vw << drawing;

//     std::stringstream ss;
//     ss << count;
//     filename = ss.str() + ".png";
//     imwrite(filename,drawing, compression_params);


     /* PATHING
      * Jonathan Holland
      * 11/04/14
      */

     // Use the <vector<vector<Point>>> contours frame to extract the two biggest <vector<Point>>

     // Sort by size
     sort(contours.begin(),contours.end(),less_vectors);

     // Assign the two tracks to the two largest contours
     vector<Point> largest1 = contours[contours.size()-1];
     vector<Point> largest2 = contours[contours.size()-2];

     // Get the center of the vehicle given the AR tag has been recognised
     cv::Point2f carCenter = Markers[0].getCenter();



    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
//  waitKey();
//  return 0;
}

bool less_vectors(const std::vector<Point>& vec1, const std::vector<Point>& vec2) {
        return vec1.size() < vec2.size();
}
