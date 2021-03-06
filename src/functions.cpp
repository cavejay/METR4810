/*
 * functions.cpp
 *
 *  Created on: 28/03/2014
 *      Author: Michael
 */
#include "functions.h"

void Draw_Circles(Mat& img, const vector<Vec3f>& circles)
{
  cout << "Draw " << circles.size() << " of dem circles" << endl;

  for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
//      cout << radius <<endl;
      // draw the circle center
      circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // draw the circle outline
      circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
}


void Dilation(const Mat& src, Mat& dst, ED_SHAPE dilation_shape, double dilation_size)
{
  int dilation_type;
  if( dilation_shape == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_shape == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_shape == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  cv::dilate( src, dst, element);
}


void Erosion(const Mat& src, Mat& dst, ED_SHAPE erosion_shape, double erosion_size)
{
  int erosion_type;
  if( erosion_shape == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_shape == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_shape == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
  /// Apply the erosion operation
  erode( src, dst, element);
}

bool less_vectors(const std::vector<Point>& vec1,const std::vector<Point>& vec2)
 {
   return vec1.size() < vec2.size();
 }

string int2str(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/**
 * Returns the minimum radius around carCenter that provides at least
 * preferedPoints points in both (total) inside and outside
 * with a minimum of sideMinimum points on either.
 */
int getSearchRadius(int preferedPoints, int sideMinimum, Point2f carCenter, vector<Point> outside, vector<Point> inside)
{
  int value = 0;
  int totalPoints = 0;
  int circRadius = 0;
  int Points1 = 0;
  int Points2 = 0;
  while(totalPoints < preferedPoints || Points1 < sideMinimum || Points2 < sideMinimum){
    totalPoints = 0;
    Points1 = 0;
    Points2 = 0;

    for (size_t i = 0; i < outside.size(); i++)
    {
      // Use pythagoras on the 2 dimensional plane to find the distances
      value = sqrt(
      pow((outside[i].x - carCenter.x), 2)
      + pow((outside[i].y - carCenter.y), 2));
      // If the value is within the circle radius
      if (value < circRadius)
      {
	Points1 += 1;
      }
    }
    for (size_t i = 0; i < inside.size(); i++)
    {
      // Use pythagoras on the 2 dimensional plane to find the distances
      value = sqrt(
      pow((inside[i].x - carCenter.x), 2)
      + pow((inside[i].y - carCenter.y), 2));
      // If the value is within the circle radius
      if (value < circRadius)
      {
	Points2 += 1;
      }
    }
    totalPoints = Points1 + Points2;
    circRadius++;
  }
  return circRadius;
}















