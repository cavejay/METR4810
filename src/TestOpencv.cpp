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
  cv::Mat frame_bgr, frame_hsv, frame_gry, frame_cny, ThresTrack;
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
//  cv::namedWindow("Basic Stream",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  /*  This is where we put all the looping stuff.
   *  I'm thinking only a single thread atm.
   *  The for(;;) also will go just above here :) */

    // Gray
    cv::cvtColor( frame_bgr, frame_gry, cv::COLOR_BGR2GRAY);
    cv::imshow( "Example Gray", frame_gry );

    // Canny
    cv::Canny( frame_gry, frame_cny, 10, 100, 3, true );
    cv::imshow("Example Canny", frame_cny);

    // Track as black
    cv::cvtColor(frame_bgr, frame_hsv, cv::COLOR_BGR2HSV);
    frame_hsv = frame_bgr;
    vector<Mat> hsvchannels(frame_hsv.channels());
    cout << frame_hsv.channels() << endl;
    cv::split(frame_hsv, hsvchannels);
    imshow("H", hsvchannels[0]);
    imshow("V", hsvchannels[2]);
    cv::threshold(hsvchannels[2], ThresTrack,150,255,cv::THRESH_BINARY);
    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,0.5);
    Erosion(ThresTrack,ThresTrack,ED_RECTANGLE,0.3);
    Dilation(ThresTrack,ThresTrack,ED_RECTANGLE,0.8);
    imshow("Threshold Test", ThresTrack);

    //Circle
    cvtColor(frame_bgr, frame_gry, CV_BGR2GRAY);
    // smooth it, otherwise a lot of false circles may be detected
    GaussianBlur( frame_gry, frame_gry, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles(frame_gry, circles, CV_HOUGH_GRADIENT, 2, 10, 200, 100, 0,200);
    Draw_Circles(frame_bgr,circles);
    imshow( "circles", frame_bgr);





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
