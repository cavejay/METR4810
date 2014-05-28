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
#include "startup.h"
#include "functions.h"
#include "Rotate3d.h"
#include "VStream.h"
#include "RobotSim.h"
#include "PID.h"
#include "sys_constants.h"
#include "race_track_extraction.h"
#include "fill_black.h"
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
  std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
	    << "Options:\n"
	    << "\t-h,--help\t\tShow this help message\n"
	    << "\t-f,--file <filename>\tLoad the following settings and more, from a .yml file\n"
	    << "\t-g,--generatefile\tCreates a settings file with the default settings\n"
	    << "\t-s,--inputsource <input here>\tCan be 'roborealm', 'still', 'video' or 'camera'\n"
	    << "\t-H,--host <HOST IP>\tSpecify the IP Address of the Roborealm Server\n"
	    << "\t-d,--destination <DESTINATION>\tSpecify the path for the image or video file\n"
	    << "\t-c,--cameranumber <CAMERANUMBER>\tSpecify which system camera to use. 0 is default\n"
	    << "\t-sim,--showsimulation \tShows a simulation running on the given input source. This will ignore the car\n"
	    << "\t-demo,--demonstration \tRuns a demo using the simulation and a pre-determined picture\n"
	    << std::endl;
	    std::cin.get();
}

void CallBackFunc(int event, int x, int y, int flags, void *ptr)
{
	Point* point = (Point*)ptr;
	if (event == EVENT_LBUTTONDOWN)
	{
		cout<<"left button of the mouse is clicked (" << x << "," << y <<")" <<endl;
		point->x = x;
		point->y = y;
	}

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
  } else if (in.loadFile){
    // do the things that load le settings from le file. :3
  }

  // This overrides the cmdline for nowz
  in.inputSource = "still";
  in.camera_number = 0;
  in.file_location = "Sample_Pictures/inPlace1.jpg";


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

  //GUI for software
  int start_stop_bar = 0;
  int pit_enter_exit_bar = 0;
  namedWindow("SOFTWAREAAAAAAAA",1);
  //For start/stop bar, 0 = stop, 1 = start
  createTrackbar("Start/Stop","SOFTWAREAAAAAAAA",&start_stop_bar,1,0,0);
  /*For Pit bar, 0 = no pit, 1 = enter pit, 2 = exit pit. Slide bar goes back to
   * have a value of 0 once pit exit finishes (have not implemented yet)
   */
  createTrackbar("Pit No/enter/exit","SOFTWAREAAAAAAAA",&pit_enter_exit_bar,2,0,0);



  namedWindow("Contours", CV_WINDOW_FREERATIO);
//  cv::createTrackbar( "Threshold Value", "Contours", &threshMag, 255, NULL );

   /*
    *  END of Setup
    */

  /*
   * BOB's Thresholding
   *
   */
    Point xyz;
    Mat a = Vs.pullImage(6060);
    cvtColor(a, a, CV_BGR2GRAY);
    a = race_track_extraction(a);
    namedWindow("f_bw",WINDOW_AUTOSIZE);
    imshow("f_bw", a);
    setMouseCallback("f_bw",CallBackFunc,&xyz);

    if(waitKey(300000) == 32)
    a = fill_black(a,xyz);
    imshow("f_bw", a);




   /*
    *  BEGINNING of functionality
    */

  int circleReset = 4;

  // Start Loop
  while(1)
  {
    // Grab current image from specified source
    img = Vs.pullImage(6060);

    // Check if image is empty.
    if (!img.empty())
    {
      // Lets look at it?
      cv::imshow("Grabbed Image", img);
      cout << "The image has been procured successful\n";
    } else {
      // The image was empty :(
      return -1;
    }

    frame_bgr = img.clone();




    /*
     * THRESHOLD IMAGE
     * Michael Ball
     * Updated by Jonathan Holland
     *
     */
//    if(Vs._inputFormat != ROBOREALM){
    cv::cvtColor(frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    equalizeHist( frame_gry,frame_gry );
//    } else {
//    	frame_gry = frame_bgr;
//    }
    cv::threshold(frame_gry, ThreshTrack, threshMag, 255, THRESH_BINARY);
    imshow("threshed'", ThreshTrack);
    // Contours are a vector of vectors of points
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::findContours(ThreshTrack, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0)); // Changed CV_CHAIN_APPROX from simple to none
    cv::Mat drawing = cv::Mat::zeros(ThreshTrack.size(), CV_8UC3);

    // Sort vectors
    sort(contours.begin(), contours.end(), less_vectors);

    // Make more colours for the largest vectors
    vector<Scalar> colors;
    colors.push_back(Scalar(100,100,100));
    colors.push_back(Scalar(200,200,200));
    for (int i = 0; i < contours.size()-2; i++)
    {
      Scalar color = Scalar(0, 255, 100);
      drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
    }
    drawContours(drawing, contours, contours.size()-2, colors[0], 1, 8, hierarchy, 0, Point());
    drawContours(drawing, contours, contours.size()-1, colors[1], 1, 8, hierarchy, 0, Point());

    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);

    //Draw image
    imshow("Contours", drawing);

    // Wait for 'esc' key press for 30ms.
    // If 'esc' key is pressed, break loop
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
