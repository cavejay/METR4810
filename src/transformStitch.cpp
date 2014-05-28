/*
 * transformStitch.cpp
 *
 *  Created on: 28/05/2014
 *      Author: Michael
 */

#include "transformStitch.h"

transformStitch::transformStitch() {
	// TODO This should go through all the different camera's and find the transformation matrices for them
	// and then add them to cameraTransforms

}

/**
 * MAY NOT NEED TO BE USED
 */
void transformStitch::stitch(Mat& src1, Mat& src2, Mat& dst)
{

}

/**
 * Returns the transformation matrix of camera at Port
 * Should also also store
 */
Mat transformStitch::cameraTransform(int Port)
{
	return cameraTransforms[Port];
}
