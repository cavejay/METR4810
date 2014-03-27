//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//void main( int argc, const char** argv )
//{ // Run this from the command line with no inputs atm.
//  // initialise a whole bunch of stuff
//
//
//  for(;;)
//    { /*  This is where we put all the looping stuff.
//	  I'm thinking only a single thread atm. */
//
//
//      waitKey(0); //wait infinite time for a keypress
//      destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"
//    }
//}

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture cap(0); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    for(;;)
    {
        Mat frame;

        cap >> frame;

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }
    return 0;
}
