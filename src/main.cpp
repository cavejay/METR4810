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
#include <map>
// Our .h's
#include "startup.h"
#include "CarLocator.h"
#include "functions.h"
#include "VStream.h"
#include "RobotSim.h"
#include "matlab.h"
#include "PID.h"
#include "sys_constants.h"
#include "race_track_extraction.h"
#include "matlab.h"
#include "preProc.h"
#include "birds_eye.h"

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
	in.inputSource = "roborealm";
	in.host = "169.254.88.211";
	in.ports = 6060;
	in.cameraID = 1;
	in.numCameras = 2;
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

	// Start the Pre-processing
	preProc proc = preProc();

  	cout << "Attempting Pre-Processing" << endl;
	Point fillAt;
	map<int, Mat> finalThresholds;
	// pull an image from source
	for(int i = in.ports; i < (in.numCameras+in.ports); i++){
		try {
		Mat preproc = Vs.pullImage(i);

			// extract the track as good as you can
			preproc = race_track_extraction(preproc);

			// Keep this loop going until it's broken
			while (1) {
				destroyWindow("f_bw");
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

				  finalThresholds[i] = preproc;
				  break;
				}
			}
		} catch (...) {
			cerr << "Port " << i << "failed to be retrieved" << endl;
		}
	}


	/*  THIS DOESN'T WORK FOR SOME REASON
	cout << "Which Camera's view contains the starting lights?" << endl;
	String startingLightsPortStr; int startingLightsPort;
	for(int i = in.ports; i < (in.numCameras+in.ports); i++){
		String windowName = "Camera @ Port #" + int2str(i);
		imshow(windowName, Vs.pullImage(i));
	}
	waitKey();
	cout << "Please enter the port number of the camera the starting lights are in" << endl;
	cin >> startingLightsPort;
	cout << "Good Person" << endl;
	for(int i = in.ports; i < (in.numCameras+in.ports); i++){
			String windowName = "Camera @ Port #" + int2str(i);
			destroyWindow(windowName);
	}*/ int startingLightsPort = 6060;


	cout << "Attempting Selection and Transformation of images" << endl;
	map<int, Mat> cameraTransforms, transformedIMGs, preProcIMGs;
	Mat preTransform, postTransform;
	Point checkerboardSquare;
	vector<Point2f> checkerboardPoints;
	Mat checkerboard = imread("Sample_Pictures/checkerboard.jpg");
	//checker board: 4*8 tiles, each tile have 100*100 pixels
	for (int portNum = in.ports; portNum < (6062); portNum++ ){
		// Get an image from the camera
		preTransform = Vs.pullImage(portNum);

		// Store the image for use later
		preProcIMGs[portNum] = preTransform;

		// Make the window Names
		String originalWindowName = "Camera at " + int2str(portNum);
		String transformWindowName = "Transformed Image from " + int2str(portNum);

		// Make the window and assign a mouse callback
		namedWindow(originalWindowName,CV_WINDOW_AUTOSIZE);
		setMouseCallback(originalWindowName, grabClickPointVector, &proc);

		// Create the checkerboard window, assign a mouse callback and show us the picture
		namedWindow("Checkerboard",CV_WINDOW_AUTOSIZE);
		setMouseCallback("Checkerboard", grabClickPoint, &checkerboardSquare);
		imshow("Checkerboard",checkerboard);

		// fill postTransform with black
//		postTransform = Mat::zeros(486,965,CV_32F);

		// Show image
		imshow(originalWindowName, preTransform);

		// Fill the picture frames
		cout << "Click the corresponding points of the picture \n"
				<< "and a square of the checkerboard. Then press spacebar" << endl;
		if(waitKey(3000000) == 32){}
		// Get the points of the square that was clicked.
		cout << "Point on checkerboard" << checkerboardSquare << endl;
		checkerboardPoints = findCorner(checkerboardSquare);
		cout << "CheckerboardPoints: " << checkerboardPoints << endl;

		// Grab the perspective transform from the points selected
		cout << "You're matching:\n"
				<< proc.transformPoints << "\n to \n"
				<< checkerboardPoints << endl;
		cameraTransforms[portNum] = getPerspectiveTransform(proc.transformPoints, checkerboardPoints);

		cout << "The Transformation Matrix of this camera is:\n" << cameraTransforms[portNum] << endl;

		// Transform the image
		try{
			warpPerspective(preTransform, postTransform, cameraTransforms[portNum], postTransform.size());

			// Store the image
			transformedIMGs[portNum] = postTransform.clone();
			cout << "warped the perspective" << endl;

		} catch (...){cerr << "warpPerspective FAILED :(" << endl;}
		// Display the transformed image in a window and run waitKey() to show it
		namedWindow(transformWindowName,CV_WINDOW_AUTOSIZE);
		imshow(transformWindowName,postTransform);
		cout << "Wait for a spacebar" << endl;
		if(waitKey(30000) == 32){}

		destroyWindow(originalWindowName);
		destroyWindow(transformWindowName);
		destroyWindow("Checkerboard");

		cout << "clear the point vectors for the next camera" << endl;
		checkerboardPoints.clear();
		proc.transformPoints.clear();
	}


	cout << "Piecing all the images together" << endl;
	/**
	 * Combine the the images
	 */

	Mat totalTrack = finalThresholds[in.ports];


	/**
	 * Find the Contours
	 */
	cout << "Finding the Contours of the track for the path" << endl;
    // Contours are a vector of vectors of points
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::findContours(totalTrack, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	// Sort vectors
	sort(contours.begin(), contours.end(), less_vectors);
    cv::Mat drawing = cv::Mat::zeros(totalTrack.size(), CV_8UC3);

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

    // Assign the two tracks to the two largest contours
    vector<Point> largest1 = contours[contours.size() - 1];
    vector<Point> largest2 = contours[contours.size() - 2];

    waitKey(30);

	cout << "Initialising Matlab interface" << endl;
	// reference matlab setup here
	Engine *ep = matConnBlue();
	matSend(ep, "1");
	/*
	 *  END of Setup
	 */

	cout << "GUI is being initiated into this realm." << cout;
	//GUI for software
	int start_stop_bar = 0;
	int pit_enter_exit_bar = 0;
	String GUIWindow = "User Interface";
	namedWindow(GUIWindow,CV_WINDOW_NORMAL);
	//For start/stop bar, 0 = stop, 1 = start
	createTrackbar("Start/Stop",GUIWindow,&start_stop_bar,1,0,0);
	/*
	* For Pit bar, 0 = no pit, 1 = enter pit, 2 = exit pit. Slide bar goes back to
	* have a value of 0 once pit exit finishes (have not implemented yet)
	*/
	createTrackbar("Pit No/enter/exit",GUIWindow,&pit_enter_exit_bar,2,0,0);

   /*
    *  Wait for Starting light
    */
	if(in.startingLights == true){
		int startingLightsValue = 0;
		while(startingLightsValue < 2) {
			startingLightsValue = birds_eye(Vs.pullImage(startingLightsPort),60,60,60);
		}
	}

	// Close all dem windows
	destroyAllWindows();

    // Initialise variables
    cv::Mat img, img1;
    cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
    int threshMag = 160;
    Point carPoint;

	int circleReset = 4;
	int steering = 127;
	// Start Loop
	cout << "Starting runtime loop" << cout;
	while(1) {
	// Make the frame for showing output
	cv::Mat drawing = cv::Mat::zeros(totalTrack.size(), CV_8UC3);

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

    // Try to find the car
    cvtColor(frame_bgr, frame_hsv, CV_BGR2HSV);
    carPoint = cl.findCar(frame_hsv);
    Rsim.Position = carPoint;



    /*
     * PATHING
     * Jonathan Holland
     * Updated by Michael Ball & Xavier Casley
     */

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

		// -35 to 35 are the pid.p_i_d() values
		// Therefore multiply by 3.62
		// Add 127
		int pidTurnNum = (4*-pid.p_i_d());
		steering = steering + pidTurnNum;
		if (steering>254) {
			steering = 254;
		}
		else if(steering < 1) {
			steering = 1;
		}
		string pidTurnString = static_cast<ostringstream*>( &(ostringstream() << steering) )->str();

		if ((pNum1 >= straightThreshold * pNum2 && pNum1 <= pNum2) || (pNum1 >= pNum2 && pNum1 <= straightThreshold * pNum2)) {

			// Tell the car to go straight
			matSend(ep, "200");
			matSend(ep, pidTurnString);

			// Tell the simulation car to go straight
			Rsim.move(forwardSpeed, 0);
			cout << "Go Straight\n";
		} else if (pNum2 > pNum1) {
			// Tell the car to go left

			matSend(ep, "200");
			matSend(ep, pidTurnString);

			// Tell the simulation car to go left
			Rsim.move(forwardSpeed, 4 * pid.p_i_d());
			cout << "Turning Left\n";
		} else if (pNum1 > pNum2) {


			// Tell the car to go right
			matSend(ep, "200");
			matSend(ep, pidTurnString);

			// Tell the simulation car to go right
			Rsim.move(forwardSpeed, 4 * pid.p_i_d());
			cout << "Turning right\n";
		} else {
			cout << "#####\nELSE HAPPENED\n#####" << endl;
		}

		Rsim.draw(drawing, true);      // draw the simulation

		// Give information to image:
		stringstream ss1;
		ss1 << "Number of Points Outside (1): " << pNum1;
		stringstream ss2;
		ss2 << "number of Points Inside (2): " << pNum2;
		putText(drawing, ss1.str(), Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5,
				Scalar(255, 255, 255));
		putText(drawing, ss2.str(), Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.5,
				Scalar(255, 255, 255));

		//Draw image
		// draw the simulation
		Rsim.draw(drawing, circRadius);
		Rsim.showDirection(drawing, pNum1, pNum2);
		imshow("Contours", drawing);

		// Wait for 'esc' key press for 30ms.
		// If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) {
			cout << "User Exit" << endl;

			break;
		}
	}

	/*
	 *  END of functionality
	 */

	// close matlab here
  	cout << "Closing Matlab" << endl;
	matDiscBlue(ep);
	return 0;
}
