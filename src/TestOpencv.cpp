// METR4810 Off-Board Software
// version: 0.01a

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "functions.h"

using namespace cv;
using namespace std;

static void help()
{ // This is here for when this becomes a command-line program :)

}

int main(int argc, char* argv[])
{ // Run this from the command line with no inputs atm.
  // initialise a whole bunch of stuff

/* The following is similar to what will be used for the actual demo, while we're working with a video stream
 * It isn't needed for now though, as we've got made as skillz and are just working with stills :)

  VideoCapture cap;
  int vidcap_result = init_videocapture(cap,VIDEO_FILE,"C:/Sample.avi");
  if (vidcap_result == -1)
  {
    return -1;
  } else
  if (vidcap_result == 0)
  {
	cout << "The Variable for VIDEO_FILE was invalid" << endl;
  } else {
	cout << "VideoCapture has been properly defined and started" << endl;
  }

*/

  Mat img1 = imread("img.jpg");

  // Initialize Variables
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny;
  if(!img1.empty())
  {
    cv::imshow("rawr",img1);
    cout << "I got it yo\n";
  } else {
    cout << "Yo, the image is empty???\n";
    return -1;
  }

  frame_bgr = img1;

  // Make le windows
  cv::namedWindow("Basic Stream",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  /*  This is where we put all the looping stuff.
   *  I'm thinking only a single thread atm.
   *  The for(;;) also will go just above here :) */

    cv::imshow("Basic Stream", frame_bgr); //show the frame in "MyVideo" window

    cv::cvtColor( frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::namedWindow( "Example Gray", cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Example Canny", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Example Gray", frame_gry );
    cv::Canny( frame_gry, frame_cny, 10, 100, 3, true );
    cv::imshow("Example Canny", frame_cny);


    /* Don't need this yet :)
    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
    */
  waitKey();
//  return 0;

}
