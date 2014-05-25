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
String rawrS[19] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19"};

Mat frame;
//  rawr.open("dfs.avi");
  // Start Loop
  int count = 0;
  cout << "whakjhf\n";

  while(count < 20)
  {
    /*
     * DETECT AR_TAG
     * Michael Ball
     *
     */
    cout << "rawrwut\n";
    aruco::MarkerDetector MDetector;
    vector<Marker> Markers;
//    try
//    {
//      //read the input image
////      cv::Mat InImage = Vs.pullImage(); //
//      cv::Mat InImage = imread("Sample_Pictures/ARTag/" + rawrS[count] + ".png");
//      // Begin detection
//      MDetector.detect(InImage, Markers);
//      // For each marker, draw info and its boundaries in the image
//      for (unsigned int i = 0; i < Markers.size(); i++)
//      {
//	cout << Markers[i] << endl;
//	Markers[i].draw(InImage, Scalar(0, 0, 255), 2);
//      }
//      cv::imshow(rawrS[count], InImage);
//     }
//    catch (std::exception &ex)
//    {
//      cout << "aruco failed\nException :" << ex.what() << endl;
//    }


    // Grab current image from specified source
//    bool bSuccess = rawr.read(frame); // read a new frame from video
//    if (!bSuccess){cout << "rawr\n";}
//    else {imshow("rawrpic", frame);}

    count++;
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
