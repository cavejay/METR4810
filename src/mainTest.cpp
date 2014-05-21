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
#include <sstream>
// Our .h's
#include "functions.h"
#include "Rotate3d.h"
#include "VStream.h"
#include "RobotSim.h"
#include "PID.h"
#include "sys_constants.h"
// Aruco .h's
#include "src/marker.h"
#include "src/aruco.h"
#include "src/cvdrawingutils.h"
// Definitions
#define ever ;;
// Namespaces
using namespace cv;
using namespace std;
using namespace aruco;

void show_usage(std::string name){
  std::cerr << "Usage: " << name << " <option(s)> SOURCES"
	    << "Options:\n"
	    << "\t-h,--help\t\tShow this help message\n"
	    << "\t-s,--inputsource <input here>\t can be 'roborealm', 'still', 'video' or 'camera'\n"
	    << "\t-H,--host <HOST IP>\tSpecify the IP Address of the Roborealm Server\n"
	    << "\t-d,--destination <DESTINATION>\tSpecify the path for the image or video file\n"
	    << "\t-c,--cameranumber <CAMERANUMBER>\tSpecify which system camera to use. 0 is default\n"
	    << "\t-mt,--multithreading \tUsing this switch will run the program in multithreading mode\n"
	    << "\t-sim,--showsimulation \tShows a simulation running on the given input source. This will ignore the car\n"
	    << "\t-demo,--demonstration \tRuns a demo using the simulation and a pre-determined picture\n"
	    << std::endl;
	    std::cin.get();
}

int main (int argc, char* argv[])
{
  if (argc > 200) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
      show_usage(argv[0]); // Inform the user of how to use the program
      exit(0);
  }

  // Translate the input commands into somthing useful
  inputVars in = getInputData(argc, argv);
  if(!in.varsParsedSuccessfully){
    exit(0);
  }

  // This overrides the cmdline for nowz
  in.inputSource = "roborealm";
  in.file_location = "Sample_Pictures/demo-track.png";


  /*
   * BEGINNING of Setup
   */
  VStream Vs(in);

  // Initialise Robo Simulation
  RobotSim Rsim = RobotSim(Point2d(600,300),0,"Robot1", 2.0,Size(45,20));

  // Initialise PID for pathing
  PID pid(0,0,0,10);

  // Initialise variables
  cv::Mat img, img1;
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
  int threshMag = 160;

  namedWindow("Contours", CV_WINDOW_FREERATIO);
//  cv::createTrackbar( "Threshold Value", "Contours", &threshMag, 255, NULL );

   /*
    *  END of Setup
    */


   /*
    *  BEGINNING of functionality
    */

  // Start Loop
  while(1)
  {
    // Grab current image from specified source
    img = Vs.pullImage(6060);

    // Check if image is empty.
    if (!img.empty())
    {
      cv::imshow("Grabbed Image", img);
      cout << "The image has been procured successful\n";
    } else {
      return -1;
    }

    frame_bgr = img.clone();

    /*
     * THRESHOLD IMAGE + RUN ROBOSIM AS A TEST
     * Michael Ball
     * Updated by Jonathan Holland
     *
     */
    cv::cvtColor(frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::threshold(frame_gry, ThreshTrack, threshMag, 255, THRESH_BINARY);

    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", img);


    if (waitKey(30) == 27)
    {
      cout << "User Exit" << endl;
      break;
    }
  }

  /*
   *  END of functionality
   */


return 0;
}
