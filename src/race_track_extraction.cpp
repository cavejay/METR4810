/*
 * race_track_extraction.cpp
 *
 *  Created on: 2014Äê5ÔÂ24ÈÕ
 *      Author: Administrator
 */

#include "race_track_extraction.h"


/**
 * Attempts to extract the track from an image
 * Requires a full colour bgr image
 */
Mat race_track_extraction(const Mat& img)
{
	Mat img_bw, img_gry;
	// make a grayscale img
	cvtColor(img, img_gry, CV_BGR2GRAY);
	// Check validity of data
	if(!img_gry.data){
		cerr << "Image did not contain usable data\n Quiting program" << std::endl;
		exit(EXIT_FAILURE);
	}
	threshold(img_gry,img_bw,100,255,THRESH_BINARY);

	// Show us what it looks like
	namedWindow("after_thresholding",WINDOW_AUTOSIZE);
	imshow("after_thresholding", img_bw);

	//
	removeSmallBlobs(img_bw, 2000);
	Mat SE = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,3),Point(-1,-1));
	morphologyEx(img_bw,img_bw,MORPH_OPEN,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
	morphologyEx(img_bw,img_bw,MORPH_CLOSE,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
	// invert the image
	bitwise_not(img_bw,img_bw);

	removeSmallBlobs(img_bw, 10000);
	namedWindow("after_complement",WINDOW_AUTOSIZE);
	imshow("after_complement", img_bw);
	Mat SE_square = getStructuringElement(MORPH_RECT,cv::Size(8,8),Point(-1,-1));
	dilate(img_bw,img_bw,SE_square);
	erode(img_bw,img_bw,SE_square);
	removeSmallBlobs(img_bw, 90000);

	return img_bw;
}

void fill_black(Mat& image,Point p)
{
	floodFill(image, p, 0);
}

void removeSmallBlobs(Mat image, double size)
{
	/* Code replacement for bwareaopen in matlab
	 * Input binary image only
	 */
	std::vector<std::vector<cv::Point> >contours;
	// find the contours of the image
	cv::findContours(image.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	// for all the contours check if they're under a certain size in area
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		// if they are smaller than size then colour them in black
		if (area > 0 && area <= size)
		{
			// this colours them in black
			cv::drawContours(image,contours,i,0,-1);
		}
	}
}

