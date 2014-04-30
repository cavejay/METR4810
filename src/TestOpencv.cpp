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

  VStream Vs(STILL_IMAGE, "127.0.0.1", "Sample_Pictures/track-example3.png");
  //  Vs.FindInput();
  //  initialise the appropriate video device. This is kinda messy but needed because I need it 4lulz and modularity
  Vs.StartInput();

  // Make Robot Simulator
  RobotSim Rsim = RobotSim(Point2d(600,300),0,"Robot1",Size(30,15));

//  VideoWriter Vw("Test1.avi",-1,30,Size(838,670));


  ////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////

  // Initialise all dem variable
  cv::Mat img, img1;///== imread("img.jpg");
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThreshTrack;
  int threshMag = 160;

  vector<float> error_list;

  vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);
  namedWindow("Contours", 1);
  cv::createTrackbar( "Threshold Value", "Contours", &threshMag, 255, NULL );

  // Start Loop
  for (ever)
  {
    // Grab current image from which ever source has been specified.
    img = Vs.pullImage();

    // Check if the image is empty. There's not point continuing if so.
    if (!img.empty())
    {
      cv::imshow("Grabbed Image", img);
      cout << "I got it yo\n";
    }
    else
    {
      cout << "Yo, the image is empty???\n";
      return -1;
    }
    frame_bgr = img.clone();

    /* THRESHOLD IMAGE + RUN ROBOTSIM AS A TEST
     * Michael Ball
     * 11/4/14
     */


    cv::cvtColor(frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::threshold(frame_gry, ThreshTrack, threshMag, 255, THRESH_BINARY);

    // Contours are a vector of vectors of points
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::findContours(ThreshTrack, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0)); // Changed CV_CHAIN_APPROX from siple to none
    cv::Mat drawing = cv::Mat::zeros(ThreshTrack.size(), CV_8UC3);
    // Sort le vectors
    sort(contours.begin(), contours.end(), less_vectors);
    // Make more colours for the largest vecotrs
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

    /*	DETECT AR_TAG
     * Michael Ball
     * 11/4/14
     */
    /*
    aruco::MarkerDetector MDetector;
    vector<Marker> Markers;
    try// This is only here 'cause its how the aruco library's dev's wrote their sample
    {
      //read the input image
//       cv::Mat InImage = Vs.pullImage(); //
      cv::Mat InImage = imread("Sample_Pictures/Marker1.png");
      //Ok, let's detect
      MDetector.detect(InImage, Markers);
      //for each marker, draw info and its boundaries in the image
      for (unsigned int i = 0; i < Markers.size(); i++)
      {
	cout << Markers[i] << endl;
	Markers[i].draw(InImage, Scalar(0, 0, 255), 2);
      }
//      cv::imshow("in", InImage);
      //        cv::waitKey(0);//wait for key to be pressed
    }
    catch (std::exception &ex)
    {
      cout << "aruco failed\nException :" << ex.what() << endl;
    }
    */

    /* PATHING
     * Jonathan Holland
     * 11/04/14
     */

    // Use the <vector<vector<Point>>> contours frame to extract the two biggest <vector<Point>>
    // Sort by size
//    sort(contours.begin(), contours.end(), less_vectors);

    // Assign the two tracks to the two largest contours
    vector<Point> largest1 = contours[contours.size() - 1];
    vector<Point> largest2 = contours[contours.size() - 2];

    // Get the center of the vehicle given the AR tag has been recognised
//    cv::Point2f carCenter = Markers[0].getCenter();
    cv::Point2f carCenter = Rsim.Position;
    double average1 = 0;
    double average2 = 0;

    double value = 0;
    double totalValue1 = 0;
    double totalValue2 = 0;
    int count = 1;
    int Pnum1 = 0;
    int Pnum2 = 0;
    float avg_error = 0;

    // Assign a value to make the circle around the car
    double circRadius = 50;

    // Probably the outside of the track
    for (int i = 0; i < largest1.size(); i++)
    {
      // Use pythagoras on the 2 dimensional plane to find the distances
      value = sqrt(
	  pow((largest1[i].x - carCenter.x), 2)
	  + pow((largest1[i].y - carCenter.y), 2));
      // If the value is within the circle radius of a distance
      if (value < circRadius)
      {
	totalValue1 += value;
	count += 1;
      }
    }
    average1 = totalValue1 / count;
    Pnum1 = count;

    value = 0;
    count = 1;

    // Do the same for largest2, the other contour probably the inside of the track
    for (int i = 0; i < largest2.size(); i++)
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
    Pnum2 = count;

    // Calculate error from |average1 - average2|
    avg_error = fabs(average1 - average2);
    // Proportional
    float p = avg_error*0.6;
    error_list.push_back(avg_error);
    float integrate = 0;
    if(error_list.size() > 4)
    {
      for(int i = 0;i < error_list.size()-1;i++)
      {
	integrate += (error_list[i] + error_list[i+1])/2;
      }
      cout << "integrate: " << integrate << endl;
    }
    float p_i = integrate*0.001 + p;

    /* Info dump */
    cout <<
	"Total Distance Outside (1): " << totalValue1 << endl <<
	"Total Distance Inside (2): " << totalValue2 << endl <<
	"Average Outside (1): " << average1 << endl <<
	"Average Inside (2): " << average2 << endl <<
	"Average Error: " << avg_error << endl <<
	"Number of Points Outside (1): " << Pnum1 << endl <<
	"number of Points Inside (2): " << Pnum2 << endl;


    // If one average is larger than the other, move towards that edge
    // If differences are tiny just go straight
    if((Pnum1 >= .99*Pnum2 && Pnum1 <= Pnum2 ) || (Pnum1 >= Pnum2 && Pnum1 <= .99*Pnum2))
    {
      // Tell the car to go straight
      Rsim.move(2 , 0);// Move the simulation
      cout << "Go Straight\n";
    }
    // Assuming largest2 is the inside track (being smaller than largest 1)
    else if (Pnum1 > Pnum2)
    {
      // Tell the car to go right
      Rsim.move(2, 4);// Move the simulation
      cout << "Turning right\n";
    }
    else if (Pnum2 > Pnum1)
    {
      // Tell the car to go left
      Rsim.move(2, -4); // Move the simulation
      cout << "Turning Left\n";
    } else {
      cout << "#####\nELSE HAPPENED\n#####" << endl;
    }


    Rsim.draw(drawing, circRadius);// draw the s`imulation

    // Give information to image:
    stringstream ss1;
    ss1 <<  "Number of Points Outside (1): " << Pnum1;
    stringstream ss2;
    ss2 <<  "number of Points Inside (2): " << Pnum2;
    putText(drawing, ss1.str(),Point(10,30), FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255));
    putText(drawing, ss2.str(),Point(10,60), FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255));

    //Draw image
    imshow("Contours", drawing);

    if (waitKey(1) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }

//  waitKey();
//  return 0;
}
