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
	imshow("original", img);
	threshold(img_gry,img_bw,100,255,THRESH_BINARY);
	imshow("gray", img_gry);
	imshow("thresh", img_bw);
	// Show us what it looks like
	namedWindow("after_thresholding",WINDOW_AUTOSIZE);
	imshow("after_thresholding", img_bw);

	// remove the smallest contours of the image
	removeSmallBlobs(img_bw, 2000);

	// Make an element for use in morphologyEx
	Mat SE = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,3),Point(-1,-1));

	// Open and close the image to remove smaller holes
	morphologyEx(img_bw,img_bw,MORPH_OPEN,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
	morphologyEx(img_bw,img_bw,MORPH_CLOSE,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());

	// invert the image
	bitwise_not(img_bw,img_bw);

	// Remove more blobs?
	removeSmallBlobs(img_bw, 10000);

	// Make an element for dilation and erosion
	Mat SE_square = getStructuringElement(MORPH_RECT,cv::Size(8,8),Point(-1,-1));

	// Do the dilation and erosion which is practically a open or close function
	dilate(img_bw,img_bw,SE_square);
	erode(img_bw,img_bw,SE_square);

	// Then remove more small objects
//	removeSmallBlobs(img_bw, 90000);

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

