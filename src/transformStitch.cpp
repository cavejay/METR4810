/*
 * transformStitch.cpp
 *
 *  Created on: 28/05/2014
 *      Author: Michael
 */

#include "transformStitch.h"

camTrans::camTrans(VStream Vs, inputVars in) {
	// TODO This should go through all the different camera's and find the transformation matrices for them
	// and then add them to cameraTransforms

	// Grab the port numbers we're looking at
	int startingPort = Vs.startingPort();
	int last = startingPort + in.numCameras;
	while(startingPort < last){
		ports.push_back(startingPort);
		startingPort++;
	}

	// Something to hold the images
	// Pull images from each of the cameras and place in a map.
	// Images are labelled with their port number
	for (int i =0; i < ports.size(); i ++ ){
		cameraIMGs[ports[i]] = Vs.pullImage(ports[i]);
		String windowName = "Camera at " + int2str(ports[i]);
		namedWindow(windowName,CV_WINDOW_KEEPRATIO);
		setMouseCallback(windowName,mouseHandler, NULL );
		imshow(int2str(ports[i]), cameraIMGs[ports[i]]);
		if(waitKey() == 32){}

	}


}

/**
 * MAY NOT NEED TO BE USED
 */
void camTrans::stitch(Mat& src1, Mat& src2, Mat& dst)
{

}

/**
 * Returns the transformation matrix of camera at Port
 * Should also also store
 */
Mat camTrans::cameraTransform(int Port)
{
	return cameraTransforms[Port];
}

void camTrans::mouseHandler(int event, int x, int y, int flags, void* params){

}
