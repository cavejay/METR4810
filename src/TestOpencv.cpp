// METR4810 Off-Board Software
// version: 0.01a

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{ // Run this from the command line with no inputs atm.
  // initialise a whole bunch of stuff

  cv::VideoCapture cap(0); // open the video camera no. 0
  if (!cap.isOpened())  // if not success, exit program
  {
    cout << "Cannot open the video cam\nAttempting to Load Sample.avi" << endl;
    VideoCapture cap("C:/Users/SHERMAL/Desktop/SampleVideo.avi"); // Load Video
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    cout << "Frame per seconds : " << fps << endl;

  }
  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
  // Show what you're reporting.
  cout << "Frame size : " << dWidth << " x " << dHeight << endl;

  // Initialize Variables
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny;

  // Make le windows
  cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  for(;;)
  { /*  This is where we put all the looping stuff.
	I'm thinking only a single thread atm. */
    cap >> frame_bgr;
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
