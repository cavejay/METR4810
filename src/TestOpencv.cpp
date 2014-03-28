// METR4810 Off-Board Software
// version: 0.01a

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "functions.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{ // Run this from the command line with no inputs atm.
  // initialise a whole bunch of stuff

  VideoCapture cap;
  init_videocapture(cap,VIDEO_FILE, "C:/Sample.avi");

  // Initialize Variables
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny;

  // Make le windows
  cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  for(;;)
  { /*  This is where we put all the looping stuff.
	I'm thinking only a single thread atm. */


    bool bSuccess = cap.read(frame_bgr);
    if (!bSuccess) //if not success, break loop
    {
                      cout << "Cannot read the frame from video file" << endl;
                     break;
    }
    cv::imshow("Basic Stream", frame_bgr); //show the frame in "MyVideo" window

    cv::cvtColor( frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::namedWindow( "Example Gray", cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Example Canny", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Example Gray", frame_gry );
    cv::Canny( frame_gry, frame_cny, 10, 100, 3, true );
    cv::imshow("Example Canny", frame_cny);

    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}
