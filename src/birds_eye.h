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

using namespace cv;
using namespace std;

Mat birds_eye(Mat image,Point top_l, Point top_r, Point bot_l, Point bot_r);


#endif /* BIRDS_EYE_H_ */
