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

int main (void) // int argc, char* argv[]
{

   /*
	*  BEGINNING of Setup
	*/
  VStream Vs(STILL_IMAGE, "127.0.0.1", "Sample_Pictures/track-example5.png");

  //  Initialise the video device
  Vs.StartInput();

  // Initialise Robo Simulation
  RobotSim Rsim = RobotSim(Point2d(600,300),0,"Robot1", 2.0,Size(30,15));


  // Initialise variables
  cv::Mat img, img1;
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
  int threshMag = 160;

  vector<float> error_list;

  vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);
  namedWindow("Contours", 1);
  cv::createTrackbar( "Threshold Value", "Contours", &threshMag, 255, NULL );

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
    img = Vs.pullImage();

    // Check if image is empty.
    if (!img.empty())
    {
      cv::imshow("Grabbed Image", img);
      cout << "The image has been procured successful\n";
    }

    else
    {
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

    // Contours are a vector of vectors of points
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::findContours(ThreshTrack, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0)); // Changed CV_CHAIN_APPROX from siple to none
    cv::Mat drawing = cv::Mat::zeros(ThreshTrack.size(), CV_8UC3);

    // Sort vectors
    sort(contours.begin(), contours.end(), less_vectors);

    // Make more colours for the largest vectors
    vector<Scalar> colors;
    colors.push_back(Scalar(100,100,100));
    colors.push_back(Scalar(200,200,200));
    for (int i = 0; i < contours.size(); i++)
    {
      Scalar color = Scalar(0, 255, 100);
      drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
    }

    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);

    /*
     * DETECT AR_TAG
     * Michael Ball
     *
     */
    /*
    aruco::MarkerDetector MDetector;
    vector<Marker> Markers;
    try
    {
      //read the input image
      cv::Mat InImage = Vs.pullImage(); //
      cv::Mat InImage = imread("Sample_Pictures/Marker1.png");
      // Begin detection
      MDetector.detect(InImage, Markers);
      // For each marker, draw info and its boundaries in the image
      for (unsigned int i = 0; i < Markers.size(); i++)
      {
	cout << Markers[i] << endl;
	Markers[i].draw(InImage, Scalar(0, 0, 255), 2);
      }
      cv::imshow("in", InImage);
     }
    catch (std::exception &ex)
    {
      cout << "aruco failed\nException :" << ex.what() << endl;
    }
    */

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
    float avg_error = 0;

    // Assign a value to make the circle around the car
    double circRadius = 60;
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

    // Calculate error from |average1 - average2|
    avg_error = (pNum1 - pNum2);

    /* Info dump */
    cout <<
	"Total Distance Outside (1): " << totalValue1 << endl <<
	"Total Distance Inside (2): " << totalValue2 << endl <<
	"Average Outside (1): " << average1 << endl <<
	"Average Inside (2): " << average2 << endl <<
	"Average Error: " << avg_error << endl <<
	"Number of Points Outside (1): " << pNum1 << endl <<
	"Number of Points Inside (2): " << pNum2 << endl;

    /*
     *  Beginning of PID Controller
     *  Code by Michael
     */
    // Constants
    float pConstant = 0.1;
    float iConstant = 0.0005;
    float dConstant = 0.1;

    // Number of error values to use in the integration
    int integrationNum = 10;

    // Proportional
    float p = avg_error*pConstant;

    // Integral
    error_list.push_back(avg_error);
    float integrate = 0;
    if(error_list.size() > integrationNum)
    {
      for(size_t i = error_list.size()-(integrationNum+1); i < error_list.size()-1; i++)
      {
	integrate += (error_list[i] + error_list[i+1])/2;
      }
      cout << "integrate: " << integrate << endl;
    }
    float intgrtr = integrate*iConstant;
    float p_i = intgrtr + p;

    // Derivative (not a major problem)
    float errorDifference = error_list[error_list.size()-1] - error_list[error_list.size()-2];
    float d = errorDifference*dConstant;
    float p_i_d = p_i + d;

    /*
     * Simulation Controller (may not work for actual car)
     * Created by Jonathan Holland
     * Edited by Michael Ball and Xavier Casley
     */
    // Minimum threshold to go straight
    float straightThreshold = 0.99;
    float forwardSpeed = 1;

    // Add in some smoothing by accounting for a large increase in points (sharp turns/chicanes)
    // To do this, initialise variables:
    float pNumDiscrep = abs(pNum1-pNum2); // The difference in number of points from outside to inside of track
    float pNumDiscrepTol = 0; // The tolerance value for pNumDiscrep - requires testing
    float sampleTime = 0; // Do we need a sample time or just check if pNumDiscrep > pNumDiscrepTol every loop?
    float disableTime = 0; // The time for which to disable the turning  - requires testing


    // If pNumDiscrep > pNumDiscrepTol  (set it so that it does over chicanes/sharp turns)
    	// Option A - delay the program and move off of previous commands
    	// Option B - Send it straight (as seen just below, essentially with the same check) BUT
    	//   	   	  for a set period of time instead of just this instance of the loop



    // If one average is larger than the other, move towards that edge
    // If differences are tiny just go straight
    if((pNum1 >= straightThreshold*pNum2 && pNum1 <= pNum2 ) || (pNum1 >= pNum2 && pNum1 <= straightThreshold*pNum2))
    {
      // Tell the car to go straight

      Rsim.move(forwardSpeed , 0);// Move the simulation
//      Rsim.drive(2 , 0);// Move the simulation
      cout << "Go Straight\n";
    }
    else if (pNum2 > pNum1)
    {
      // Tell the car to go left

      Rsim.move(forwardSpeed, 4*p_i_d); // Move the simulation
//      Rsim.drive(2, -4); // Move the simulation
      cout << "Turning Left\n";
    }
    // Assuming largest2 is the inside track (being smaller than largest 1)
    else if (pNum1 > pNum2)
    {
      // Tell the car to go right

      Rsim.move(forwardSpeed, 4*p_i_d);// Move the simulation
//      Rsim.drive(2, 4);// Move the simulation
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
