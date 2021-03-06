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
#include "CarLocator.h"
#include "functions.h"
#include "Rotate3d.h"
#include "VStream.h"
#include "RobotSim.h"
#include "matlab.h"
#include "PID.h"
#include "sys_constants.h"
#include "race_track_extraction.h"
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

void grabClickPoint(int event, int x, int y, int flags, void *ptr)
{
	Point* point = (Point*)ptr;
	if (event == EVENT_LBUTTONDOWN)
	{
		cout<<"left button of the mouse is clicked (" << x << "," << y <<")" <<endl;
		point->x = x;
		point->y = y;
	}

}

void mouseHandler(int event, int x,int y, int flags, void *ptr)
{

	vector<Point>* point = (vector<Point>*)ptr;
	if (event == EVENT_LBUTTONDOWN)
	{
		Point a(x,y);
		point->push_back(a);
	}
	if (event == EVENT_RBUTTONDOWN)
	{
		point->pop_back();
	}
}

void mouseHandler_cb(int event, int x,int y, int flags, void *ptr)
{

	vector<Point>* point = (vector<Point>*)ptr;
	if (event == EVENT_LBUTTONDOWN)
	{
		Point a(x,y);
		point->push_back(a);
	}
	if (event == EVENT_RBUTTONDOWN)
	{
		point->pop_back();
	}
}

int main (int argc, char* argv[])
{
	/**
	 * Input Arguments
	 * Data from files
	 * setup and initialisation of classes
	 */

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
	  in = readSettingsFile(in.filename);
  }

  // This overrides the cmdline for nowz
  in.inputSource = "still";
  in.host = "169.254.88.211";
  in.cameraID = 0;
  in.file_location = "Sample_Pictures/inPlace2-4.jpg";

  /*
   * BEGINNING of Setup
   */
  cout << "Creating classes" << endl;

  // initialise the vision feeds with the input commands
  VStream Vs(in);

  // Initialise Robo Simulation
  RobotSim Rsim = RobotSim(Point2d(600,300),0,"Robot1", 2.0,Size(45,20));

  // Initialise PID for pathing
  PID pid(0,0,0,10);

  // Car Localisation init
  CarLocator cl(in);

/**
 * Bob Zhou
 * Updated by Michael Ball
 *
 */


//  //GUI for software
//  cout << "Making GUI" << endl;
//  int start_stop_bar = 0;
//  int pit_enter_exit_bar = 0;
//  namedWindow("SOFTWAREAAAAAAAA",1);
//  //For start/stop bar, 0 = stop, 1 = start
//  createTrackbar("Start/Stop","SOFTWAREAAAAAAAA",&start_stop_bar,1,0,0);
//  /*
//   * For Pit bar, 0 = no pit, 1 = enter pit, 2 = exit pit. Slide bar goes back to
//   * have a value of 0 once pit exit finishes (have not implemented yet)
//   */
//  createTrackbar("Pit No/enter/exit","SOFTWAREAAAAAAAA",&pit_enter_exit_bar,2,0,0);
//  namedWindow("Contours", CV_WINDOW_FREERATIO);
////  cv::createTrackbar( "Threshold Value", "Contours", &threshMag, 255, NULL );
//

  	cout << "Attempting Pre-Processing" << endl;
	Point fillAt;
	// pull an image from source
	Mat preproc = Vs.pullImage(6060);
	// Keep this loop going until it's broken
	bool loopInv = true;
	while (1  && loopInv) {
		// extract the track as good as you can
		preproc = race_track_extraction(preproc);
		// Show us the image
		namedWindow("Select a point to floodfill",WINDOW_AUTOSIZE);
		setMouseCallback("Select a point to floodfill",grabClickPoint,&fillAt);
		imshow("Select a point to floodfill", preproc);

		// waitkey to show us an image
		if(waitKey() == 32){}
		//fill the picture from point fillAt
		destroyWindow("Select a point to floodfill");
		fill_black(preproc,fillAt);
		// show use the magic
		imshow("f_bw", preproc);
		if(waitKey() == 27)
		{
		  cout << "Image preprocessing done!" << endl;
		  destroyWindow("f_bw");
		  break;
		}
	}

	cout << "Attempting Selection and Transformation of images" << endl;
	map<int, Mat> cameraIMGs;
	int ports[4] = {6060,6061,6062,6063};
	Mat temp;
	Mat temp_after;
	vector<String> pics;
	vector<Point2f> points;
	vector<Point2f> points_on_checkboard;
	  pics.push_back("Tracktual/cam11.JPG");
	  pics.push_back("Tracktual/cam21.JPG");
	  pics.push_back("Tracktual/cam31.JPG");
	  pics.push_back("Tracktual/cam41.JPG");
	  //checker board: 4*8 tiles, each tile have 100*100 pixels
	  for (int i = 0; i < pics.size(); i ++ ){
		  //creating the windows for img and update that windows
		  temp = imread(pics[i]);
		  String windowName = "Camera at " + int2str(ports[i]);
		  String windowName1 = "Camera at " + int2str(ports[i]) + "after transformation";
		  namedWindow(windowName,CV_WINDOW_AUTOSIZE);
		  namedWindow(windowName1,CV_WINDOW_AUTOSIZE);
		  Mat temp_after = Mat::zeros(486,965,temp.type());
		  imshow(windowName, temp);
		  imshow(windowName1, temp_after);
		  setMouseCallback(windowName,mouseHandler, &points);
		  //creating the windows for cb and update that windows
		  namedWindow("I'm a checkerboard",CV_WINDOW_AUTOSIZE);
		  Mat checkerboard = imread("Sample_Pictures/checkerboard.jpg");
		  cout << "asdasdasfadgadg"<<checkerboard.size() << endl;
		  imshow("I'm a checkerboard",checkerboard);
		  setMouseCallback("I'm a checkerboard",mouseHandler_cb, &points_on_checkboard);
		  if(waitKey(30000000) == 32){}
		  cameraIMGs[ports[i]] = getPerspectiveTransform(points, points_on_checkboard);
		  cout << "size of vector points : " << points.size() << endl;
		  cout << "size of vector points_on_cb : " << points_on_checkboard.size() << endl;
		  cout << "Points on img"<< points << endl;
		  cout << "Points on cb" << points_on_checkboard << endl;
		  cout << "transform matrix :" << endl << cameraIMGs[ports[i]] << endl;
		  warpPerspective(temp, temp_after, cameraIMGs[ports[i]], temp_after.size());
		  imshow(windowName1,temp_after);
		  if(waitKey(30000) == 32){}
		  destroyWindow(windowName);
		  destroyWindow(windowName1);
		  destroyWindow("I'm a checkerboard");
		  points_on_checkboard.clear();
		  points.clear();
	  }



	/*
	 *  END of Setup
	 */


   /*
    *  BEGINNING of functionality
    */
    // Initialise variables
    cv::Mat img, img1;
    cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
    int threshMag = 160;
    Point carPoint;

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
      cerr << "The Pulled image was empty" << endl;
      return -1;
    }
    frame_bgr = img.clone();


    /*
     * BOB's Thresholding
     *
     */
      Mat a;
      a = race_track_extraction(a);
      namedWindow("f_bw",WINDOW_AUTOSIZE);
      imshow("f_bw", a);


    /*
     * THRESHOLD IMAGE
     * Michael Ball
     * Updated by Jonathan Holland
     *
     */
//    if(Vs._inputFormat != ROBOREALM){
    cv::cvtColor(frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    equalizeHist( frame_gry,frame_gry );

    cv::cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);

    // Try to find the car
    carPoint = cl.findCar(frame_hsv);
    Rsim.Position = carPoint;

    cv::threshold(frame_gry, ThreshTrack, threshMag, 255, THRESH_BINARY);
    imshow("threshed'", ThreshTrack);
    // Contours are a vector of vectors of points
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::findContours(a, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0)); // Changed CV_CHAIN_APPROX from simple to none
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
    imshow("Contours", drawing);

    /*
     * PATHING
     * Jonathan Holland
     * Updated by Michael Ball & Xavier Casley
     */

    // Assign the two tracks to the two largest contours
    vector<Point> largest1 = contours[contours.size() - 1];
    vector<Point> largest2 = contours[contours.size() - 2];

    // Get the center of the vehicle given the AR tag has been recognised

    // Grabbing center from the Aruco tag marker
    // cv::Point2f carCenter = Markers[0].getCenter();

    // Grabbing center for the simulation
//    cv::Point2f carCenter = Rsim.Position;
    cv::Point2f carCenter = cl.carPoint;

    // Initialise local variables
    double average1 = 0;
    double average2 = 0;
    double value = 0;
    double totalValue1 = 0;
    double totalValue2 = 0;
    int count = 1;
    int pNum1 = 0;
    int pNum2 = 0;

    // Assign a value to make the circle around the car
    int preferedPoints = 20; // 20 is good, it gets about the same answer as 10
    int minSidePoints = 5; // keep this value under half of preferedPoints
    int circRadius;
    circleReset--;
    if(circleReset < 0)
    {
    	circRadius = getSearchRadius(preferedPoints, minSidePoints, carCenter, largest1, largest2);
    	circleReset = 4;
    }
    Rsim.set_searchRadius(circRadius);

    // Probably the outside of the track
    for (size_t i = 0; i < largest1.size(); i++)
    {
      // Use pythagoras on the 2 dimensional plane to find the distances
      value = sqrt(
	  pow((largest1[i].x - carCenter.x), 2)
	  + pow((largest1[i].y - carCenter.y), 2));
      // If the value is within the circle radius
      if (value < circRadius)
      {
    	  totalValue1 += value;
    	  count += 1;
      }
    }
    average1 = totalValue1 / count;
    pNum1 = count;

    value = 0;
    count = 1;

    // Do the same for largest2, the other contour probably the inside of the track
    for (size_t i = 0; i < largest2.size(); i++)
    {
      value = sqrt(
	  pow((largest2[i].x - carCenter.x), 2)
	  + pow((largest2[i].y - carCenter.y), 2));

      if (value < circRadius)
      {
	totalValue2 += value;
	count += 1;
      }
    }
    average2 = totalValue2 / count;
    pNum2 = count;

    if(pNum2 > 110) {
    	pNum2 = 110;
    }
    if(pNum1 > 110) {
    	pNum1 = 110;
    }

    // record error
    pid.error(pNum1 - pNum2);

    /* Info dump */
    cout <<
	"Total Distance Outside (1): " << totalValue1 << endl <<
	"Total Distance Inside (2): " << totalValue2 << endl <<
	"Average Outside (1): " << average1 << endl <<
	"Average Inside (2): " << average2 << endl <<
	"Error: " << error << endl <<
	"Number of Points Outside (1): " << pNum1 << endl <<
	"Number of Points Inside (2): " << pNum2 << endl;

    /*
     *  Beginning of PID Controller
     *  Code by Michael
     */
    // Constants
    pid.Kp(0.1);
    pid.Ki(0.0005);
    pid.Kd(0.1);

    // Number of error values to use in the integration
    pid.set_integrationLen(10);

    /*
     * Simulation Controller (may not work for actual car)
     * Created by Jonathan Holland
     * Edited by Michael Ball and Xavier Casley
     */
    // Minimum threshold to go straight
    float straightThreshold = 0.99;
    float forwardSpeed = 2;

    // If one average is larger than the other, move towards that edge
    // If differences are tiny just go straight
    if((pNum1 >= straightThreshold*pNum2 && pNum1 <= pNum2 ) || (pNum1 >= pNum2 && pNum1 <= straightThreshold*pNum2))
    {
      // Tell the car to go straight
      //move(128,0);

      // Tell the simulation car to go straight
      Rsim.move(forwardSpeed , 0);
      cout << "Go Straight\n";
    }
    else if (pNum2 > pNum1)
    {
      // Tell the car to go left
      //move(0,0);

      // Tell the simulation car to go left
      Rsim.move(forwardSpeed, 4*pid.p_i_d());
      cout << "Turning Left\n";
    }
    // Assuming largest2 is the inside track (being smaller than largest 1)
    else if (pNum1 > pNum2)
    {
      // Tell the car to go right
      //move(255,0);

      // Tell the simulation car to go right
      Rsim.move(forwardSpeed, 4*pid.p_i_d());
      cout << "Turning right\n";
    }
    else {
      cout << "#####\nELSE HAPPENED\n#####" << endl;
    }

    Rsim.draw(drawing, true);// draw the simulation

    // Give information to image:
    stringstream ss1;
    ss1 <<  "Number of Points Outside (1): " << pNum1;
    stringstream ss2;
    ss2 <<  "number of Points Inside (2): " << pNum2;
    putText(drawing, ss1.str(),Point(10,30), FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255));
    putText(drawing, ss2.str(),Point(10,60), FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255));

    //Draw image
    // draw the simulation
    Rsim.draw(drawing, circRadius);
    Rsim.showDirection(drawing, pNum1, pNum2);
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
