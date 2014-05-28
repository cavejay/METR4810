// METR4810 Off-Board Software
// version: 0.01a

#if !defined(_MT)
#error _beginthreadex requires a multithreaded C run-time library.
#endif
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
#include "bluetooth.h"
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
//// Global variable for array to send data
//// Currently of limited allocated memory size
//// - no need for stacking up more commands
//int* globalArr = new int[2];
//// A global variable to check if globalArr has new values
//// false for no change, true for changes made
//bool checkArrChange = false;
//HANDLE checkSemaphore;

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

//unsigned __stdcall pyThread(void *ptr) {
//	// Initially connect to the vehicle
//	// Initialize the Python Interpreter
//		Py_Initialize();
//		PyObject *pName, *pModule;
//
//			// Build the name object
//			pName = PyString_FromString("bthConn");
//
//			// Load the module object
//			pModule = PyImport_Import(pName);
//	while(1){
//		PyObject* service = pyConn(pModule);
//	}
//
//
//	while(1){
//
//		// If the global array has something to send
//		if(checkArrChange) {
//			DWORD dwWait;
//			// Try to enter the semaphore gate.
//			dwWait = WaitForSingleObject(
//						checkSemaphore,   // handle to semaphore
//						0L);           // zero-second time-out interval
//
//			switch (dwWait) {
//			// The semaphore object was signaled.
//				case WAIT_OBJECT_0:
//					printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
//					// Use the wrapper function sendPy to move the vehicle
//					cout<<"Sending to python: "<<globalArr[0]<<"  "<<globalArr[1];
//
//					pyConn();
//					//sendPy(service,globalArr[0],globalArr[1]);
//					// Reset checkArrChange to no changes
//				    checkArrChange = false;
//
//				    // Release the semaphore when task is finished
//
//					if (!ReleaseSemaphore(
//										checkSemaphore,  // handle to semaphore
//										1,            // increase count by one
//										NULL) )       // not interested in previous count
//					{
//						printf("ReleaseSemaphore error: %d\n", GetLastError());
//					}
//					break;
//
//				// The semaphore was nonsignaled, so a time-out occurred.
//				case WAIT_TIMEOUT:
//					printf("Thread %d: wait timed out\n", GetCurrentThreadId());
//					break;
//			}
//		}
//	}
//	return 0;
//}

//void move(int mlr, int mfb) {
//	DWORD dwWait;
//	// Try to enter the semaphore gate.
//	dwWait = WaitForSingleObject(
//	    							checkSemaphore,   // handle to semaphore
//	    							INFINITE);        // Wait until semaphore is signaled
//	switch (dwWait) {
//		// The semaphore object was signaled.
//	    case WAIT_OBJECT_0:
//	    	printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
//	    	// Add movement commands to the globalArr
//
//	    	cout<<"The global Arr is: "<<globalArr[0]<<"  "<<globalArr[1];
//
//	    	globalArr[0] = mlr; // First argument is moveleftright (0 to 255)
//	    	globalArr[1] = mfb; // Second argument is moveforwardback (0 to 255)
//	    	// AFTER globalArr has been modified, change checkArrChange to true to represent changes have ALREADY BEEN MADE
//	        checkArrChange = true;
//
//	        // Release the semaphore when task is finished
//
//	        if (!ReleaseSemaphore(
//	    							checkSemaphore,  // handle to semaphore
//	    							1,            // increase count by one
//	    							NULL) )       // not interested in previous count
//	    	{
//	        	printf("ReleaseSemaphore error: %d\n", GetLastError());
//	    	}
//	    	break;
//
//	    	// The semaphore was nonsignaled, so a time-out occurred.
//	    	case WAIT_TIMEOUT:
//	    		printf("Thread %d: wait timed out\n", GetCurrentThreadId());
//	    		break;
//	}
//}


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
  in.inputSource = "camera";
  in.camera_number = 0;
  in.file_location = "Sample_Pictures/ARTag/1.png";


  /*
   * BEGINNING of Setup
   */
  blue();
  return 1;
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

//  // Need to thread the Python interpreter for bluetooth
//  HANDLE pyBlueThread, hEvent;
//  unsigned threadID;
//  int ErrorNumber, DOSErrorNumber;
//  // Create a manual-reset nonsignaled unnamed event
//  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//  pyBlueThread = (HANDLE)_beginthreadex(NULL, // security attributes ( default if NULL )
//        						0, // stack SIZE default if 0
//        						pyThread, // Start Address
//        						&hEvent, // input data
//        						0, // creational flag ( start if  0 )
//        						&threadID); // thread ID
//
//  if (pyBlueThread == 0) {
//  	ErrorNumber = errno;
//  	DOSErrorNumber = _doserrno;
//  	cerr << "Begin thread error: " << strerror(ErrorNumber) << '\n';
//  	cerr << "Begin thread DOS error code: " << DOSErrorNumber << '\n';
//  	return 1;
//  	}
//  cout << "Thread begun\n";
//
//
//  // Need to also create a semaphore the global that is changed by both threads
//  // This is similar to mutex in linux (a way of locking access to globals when running multiple threads)
//  // Create a semaphore with initial and max counts of 1 so they can only be accessed at one time
//
//  checkSemaphore = CreateSemaphore(
//              NULL,           // default security attributes
//              1,  			  // initial count
//              1,  			  // maximum count
//              NULL);          // unnamed semaphore
//
//  if (checkSemaphore == NULL) {
//	  printf("CreateSemaphore error: %d\n", GetLastError());
//      return 1;
//  }



   /*
    *  END of Setup
    */


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
    cv::Point2f carCenter = Rsim.Position;

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
//  CloseHandle(checkSemaphore);
//  CloseHandle(pyBlueThread);
//  pyDc();
return 0;
}
