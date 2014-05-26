/*
 * Locator.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */

#include "CarLocator.h"

CarLocator::CarLocator ()
{
  // TODO figure out if anything actually needs to go here?
}

/**
 * Takes a gray picture and returns the location of the car with a red
 */
Point CarLocator::findCar(const Mat& src){
  // TODO Find the blob in v space, then find the blob in s space and use
  	//pointPolygonTest to check if the mass center of the latter is within the first.
  	// This should provide our marker (Y)

	/// Reduce the noise so we avoid false circle detection
	Mat input;
	GaussianBlur( src, input, Size(9, 9), 2, 2 );

	// Find the blob of the marker in V space!
	vector<vector<Point> > Vc;
	vector<Vec4i> Vhierarchy;
	cv::findContours(src, Vc, Vhierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

	// Find the blob in the marker in S Space!
	vector<vector<Point> > Sc;
	vector<Vec4i> Shierarchy;
	cv::findContours(src, Sc, Shierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));



	imshow("input Show",input);
	Point rawr;
	return rawr;
}

void CarLocator::findCar_SURF(const Mat& src){
//  newIMG(src);
  Mat img_object = imread("Sample_Pictures/SURF/9353.jpg", CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_scene;
  if(src.channels() > 1){cvtColor(src, img_scene, CV_BGR2GRAY);}
  else {img_scene = src;}

  if( !img_object.data || !img_scene.data )
	{ std::cout<< " --(!) Error reading images " << std::endl;}

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;
//  GaussianBlur(img_scene, img_scene, Size(5,5), 0,0);
//  GaussianBlur(img_object, img_object, Size(5,5), 0,0);

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector.detect( img_object, keypoints_object );
  detector.detect( img_scene, keypoints_scene );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ ){
	double dist = matches[i].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ ){
	if( matches[i].distance < 3*min_dist ){
	  good_matches.push_back( matches[i]);
	}
  }

	Mat img_matches;
	drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
	good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( int i = 0; i < good_matches.size(); i++ )
	{
	//-- Get the keypoints from the good matches
	obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
	scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	//-- Show detected matches
	imshow( "Good Matches & Object detection", img_matches );
}



void CarLocator::newIMG(const Mat& img){
  if(imgHistory.size() > 10){
    imgHistory.pop_back();
  }
  imgHistory.push_front(img.clone());
}

Mat CarLocator::getWAvg(void){
  return imgHistory[0];
}
