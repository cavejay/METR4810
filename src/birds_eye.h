/*
 * birds_eye.h
 *
 *  Created on: 2014Äê5ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef BIRDS_EYE_H_
#define BIRDS_EYE_H_
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/highgui.h"
// C libs
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

#include "race_track_extraction.h"

using namespace cv;
using namespace std;

int birds_eye(Mat image,int th_val,int remove_size_under, int no_of_iteration);

#endif /* BIRDS_EYE_H_ */
