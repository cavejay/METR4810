/*
 * Locator.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */

#include "CarLocator.h"

CarLocator::CarLocator (inputVars in)
{
  // TODO transfer the values needed to pass to the car finder class
}

/**
 * Takes a hsv picture and returns the location of the car with a red
 */
Point CarLocator::findCar(const Mat& src){
//  cout << "split the channels" << endl;
  vector<Mat> HSVchannels(3);
  split(src, HSVchannels);

  Mat gray;
  cvtColor(src, gray, CV_HSV2BGR);
  cvtColor(gray, gray, CV_BGR2GRAY);


  // Find the blob of the marker in V space!
  // This is the white circle of the marker
  Mat V_thresh;
  vector<vector<Point> > Vc;
  vector<Vec4i> Vhierarchy;
//  imshow("HSV - V", gray);
  cv::threshold(gray, V_thresh, 200, 255, THRESH_BINARY);
  Dilation(V_thresh, V_thresh, ED_ELLIPSE,1.8);
//  imshow("HSV - V - thresh", V_thresh);
  cv::findContours(V_thresh, Vc, Vhierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

//  cout << "found the contours in the Value channel" << endl;
//  cout << "there are this many: " << Vc.size() << endl;

  // Find the blob in the marker in S Space!
  // This is the red dot
  Mat S_thresh;
  vector<vector<Point> > Sc;
  vector<Vec4i> Shierarchy;
//  imshow("HSV - S", HSVchannels[1]);
  cv::threshold(HSVchannels[1], S_thresh, 150, 255, THRESH_BINARY_INV);
//  Dilation(S_thresh, S_thresh, ED_ELLIPSE,0.8);
  bitwise_not(S_thresh,S_thresh);
//  imshow("HSV - S - thresh", S_thresh);
  cv::findContours(S_thresh, Sc, Shierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

//  cout << "found the contours in the Saturation channel" << endl;

  // find Moments
  vector<Moments> mu(Sc.size() );
  for(size_t i = 0; i < Sc.size(); i++ ){
    mu[i] = moments( Sc[i], false );
  }
//  cout << "found moments" << endl;

  ///  Get the mass centers:
  vector<Point2f> S_masscenter( Sc.size() );
  for(size_t i = 0; i < Sc.size(); i++ ){
    S_masscenter[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
  }
//  cout << "found masscenters" << endl;
//  cout << "THere are this many: " << S_masscenter.size() << endl;

  // Check if the masscenters are inside one of the contours
  // i is the contour
  // j in the masscenter
  vector<Point2f> carPoint;
  vector<vector<Point> > carContour;
  for(int i = 0; i < Vc.size(); i++){
//    cout << "i: " << i << endl;
    for(int j = 0; j < S_masscenter.size(); j++){
//      cout << "j: " << j << endl;
      // If the contour is less than 1/4 of the image continue
      if(contourArea(Vc[i]) < 0.25*src.size().area()){

    	// Check if the masscenter is in side the contour
	    int loc = pointPolygonTest(Vc[i],S_masscenter[j],false);
//		cout << loc << endl;
		if(loc > 0){
			// if it is, check to make sure the contour it's inside is a circle
			vector<Point> approx;
			cv::approxPolyDP(Vc[i], approx, cv::arcLength(Vc[i], true) *0.01, true);
			if (isContourConvex(approx)){
			  carPoint.push_back(S_masscenter[j]);
			  carContour.push_back(approx);
			}
		}
	  }
    }
  }

  Mat show = src.clone();
  cvtColor(show, show, CV_HSV2BGR);
//  for (size_t i = 0; i < Vc.size(); i++){
//    drawContours(show, Vc, i, Scalar(0,0,255),1);
//  }
////  cout << "drawn Vc onto show" << endl;
//  for (size_t i = 0; i < Sc.size(); i++){
//    drawContours(show, Sc, i, Scalar(255,0,0),1);
//  }

  // only try to draw it if there were points found.
  if(carPoint.size() > 0)
  {
	for(size_t i = 0; i < carPoint.size(); i++){
	circle( show, carPoint[0], 20, Scalar(0,255,0), 5, 8, 0 );
	drawContours(show, carContour, i, Scalar(0,255,255));
	}
  } else {
	  cerr << "No Car was found in the current image. Previous location was: " << History[History.size()-1] << endl;
	  return Point(-1,-1);
  }

//  imshow("input Show",show);
  return carPoint[0];
}

void findMarkerContours(const Mat& src)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> approx;

	bool markerFound = false;

	cv::Mat frame = src.clone();
	Mat src_gray;
	double thresh = 160;

	cv::cvtColor(frame, src_gray, CV_BGR2GRAY);
	//Reduce noise with a 3x3 kernel
	blur( src_gray, src_gray, Size(3,3));

	//Convert to binary using canny
	cv::Mat bw;
	cv::Canny(src_gray, bw, thresh, 3*thresh, 3);

	imshow("bw", bw);

	findContours(bw.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat drawing = Mat::zeros( bw.size(), CV_8UC3 );

	for (int i = 0; i < contours.size(); i++)
	{
	  int child_count = 0;
	  Scalar color = Scalar(120, 120,120);
	  // contour
	  drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );

	  //Approximate the contour with accuracy proportional to contour perimeter
	  cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) *0.02, true);

	  //Skip small or non-convex objects
	  if(fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
		  continue;
	  if (approx.size() >= 8) //More than 6-8 vertices means its likely a circle
	  {
	    int index = i;
	    int count = 0;
	    for (int j = 0; j < 10; j++) // Check for up to 9 child contours. This is our marker.
	    // Not robust to speed or scale...
	    // Check for a small number of child contours and HSV values inside the smallest one?? YEEE
	    {
	      if(hierarchy[index][2] != -1)
	      {
		index++; // Contour has a child, move to next contour
		count++;
	      } else {
		break;
	      }
	    }
	    if (count >= 4 && count <= 6) // || HSV IS BLUE
	    {
	      drawContours( frame, contours, i, Scalar(0,255,0), 2, 8);
	    }
	  }
	}
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
