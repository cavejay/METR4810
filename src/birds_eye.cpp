/*
 * birds_eye.cpp
 *
 *  Created on: 2014Äê5ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "birds_eye.h"
//starting light recognition function
//reused birds_eye cause ima lazy nigga

int birds_eye(Mat image,int th_val, int remove_size_under, int no_of_iteration)
{
	Mat image_gray;
	vector<vector<Point> > contours;
	int no_of_blobs;
	cvtColor( image, image_gray, CV_BGR2GRAY );
//	namedWindow("starting light gray", CV_WINDOW_FREERATIO);
//	imshow("starting light gray",image_gray);
	threshold(image_gray,image_gray,th_val,255,THRESH_BINARY);
	removeSmallBlobs(image_gray,remove_size_under);
	Mat SE = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,3),Point(-1,-1));
	for (int i = 0; i < no_of_iteration; i++)
	{
		erode(image_gray,image_gray,SE);
	}
	namedWindow("lights after threshold",CV_WINDOW_FREERATIO);
	imshow("lights after threshold",image_gray);

	findContours(image_gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	no_of_blobs = contours.size();
	return no_of_blobs;
}
