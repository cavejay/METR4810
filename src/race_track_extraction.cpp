/*
 * race_track_extraction.cpp
 *
 *  Created on: 2014Äê5ÔÂ24ÈÕ
 *      Author: Administrator
 */

#include "race_track_extraction.h"

Mat race_track_extraction(Mat image)
{
	Mat f;
	Mat f_bw;
	f = image;
	if(!f.data){
		cout<<"Could not open or find the image"<<std::endl;
		exit(EXIT_FAILURE);
	}
	namedWindow("Original image",WINDOW_AUTOSIZE);
	imshow("Original image", f);
	Size s = f.size();
	int rows = s.height;
	int cols = s.width;
	threshold(f,f_bw,100,255,THRESH_BINARY);
	namedWindow("after_thresholding",WINDOW_AUTOSIZE);
	imshow("after_thresholding", f_bw);
	/*
	namedWindow("Threshold image",WINDOW_AUTOSIZE);
	imshow("Threshold image",f_bw);
	 */
	removeSmallBlobs(f_bw, 2000);
	Mat SE = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,3),Point(-1,-1));
	morphologyEx(f_bw,f_bw,MORPH_OPEN,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
	morphologyEx(f_bw,f_bw,MORPH_CLOSE,1,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());

//	for (int i = 0; i< rows; i++)
//	{
//		for (int j = 0; j < cols; j++)
//		{
//			if (f_bw.at<int>(i,j) > 1)
//			{
//				f_bw.at<int>(i,j) = 0;
//			} else {
//				f_bw.at<int>(i,j) = 255;
//			}
//		}
//	}
	bitwise_not(f_bw,f_bw);

	removeSmallBlobs(f_bw, 10000);
	namedWindow("after_complement",WINDOW_AUTOSIZE);
	imshow("after_complement", f_bw);
	Mat SE_square = getStructuringElement(MORPH_RECT,cv::Size(8,8),Point(-1,-1));
	dilate(f_bw,f_bw,SE_square);
	erode(f_bw,f_bw,SE_square);
	removeSmallBlobs(f_bw, 90000);
//	for (int i= 0; i<10; i++)
//	{
//		erode(f_bw,f_bw,SE);
//	}
//	for (int i= 0; i<10; i++)
//	{
//		dilate(f_bw,f_bw,SE);
//	}
//	vector<vector<Point> > contours;
//	Scalar color = Scalar(0, 255, 100);
	cout<<"wtf?!"<<std::endl;
//	findContours(f_bw,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
//	drawContours(f_bw,contours,-1,color,1,8,noArray(),1);

	return f_bw;
}

Mat fill_black(Mat image,Point p)
{
	floodFill(image, p, 0,0,Scalar(),Scalar(),4);
	return image;
}

void removeSmallBlobs(Mat image, double size)
{
	/*Code replacement for bwareaopen in matlab
	 * Input binary image only
	 */
	std::vector<std::vector<cv::Point> >contours;
	cv::findContours(image.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		if (area > 0 && area <= size)
		{
			cv::drawContours(image,contours,i,0,-1);
		}
	}
}

