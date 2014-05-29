/*
 * transformStitch.cpp
 *
 *  Created on: 28/05/2014
 *      Author: Michael
 */

#include "transformStitch.h"

camTrans::camTrans(VStream Vs) {
	// TODO This should go through all the different camera's and find the transformation matrices for them
	// and then add them to cameraTransforms

	// Grab the port numbers we're looking at
	ports = Vs.portNumbers();

	// Something to hold the images
	// Pull images from each of the cameras and place in a map.
	// Images are labelled with their port number
	for (int i =0; i < ports.size(); i ++ ){
		cameraIMGs[ports[i]] = Vs.pullImage(ports[i]);

		namedWindow("Camera at " + int2str(ports[i]),CV_WINDOW_KEEPRATIO);
		imshow(int2str(ports[i]), cameraIMGs[ports[i]]);



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

