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

void grabClickPoint(int event, int x, int y, int flags, void *ptr)
{
	Point* point = (Point*)ptr;
	if (event == EVENT_LBUTTONDOWN)
	{
		cout<<"left button of the mouse is clicked (" << x << "," << y <<")" <<endl;
		point->x = x;
		point->y = y;
	}

}

void grabClickPointVector(int event, int x,int y, int flags, void *ptr)
{
	preProc *proc = (preProc*)ptr;
	if (event == EVENT_LBUTTONDOWN)	{
		proc->transformPoints.push_back(Point(x,y));
		cout<<"left button of the mouse is clicked (" << x << "," << y <<")" <<endl;
	} else if (event == EVENT_RBUTTONDOWN) {
		proc->transformPoints.pop_back();
		cout << "The Last point, you clicked was removed from your selection" << endl;
	}

}


void Dilation(const Mat& src, Mat& dst, ED_SHAPE dilation_shape, double dilation_size)
{
  int dilation_type;
  if( dilation_shape == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_shape == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_shape == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ),
		  Point( dilation_size, dilation_size ) );
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

vector<Point2f> findCorner(Point xy) {
	// Checkerboard parts are 120.5 pixels wide and high

	int x1, x2, y1, y2;

	// mod the x and y of point by 100 to find how far away and then minus this remainder to get the sides
	if((xy.x % 100) != 0) {
		x1 = xy.x - (xy.x % 100);
		x2 = xy.x + (100 - (xy.x % 100));
	}
	else {
		x1 = xy.x;
		x2 = xy.x + 100;
	}

	if((xy.y % 100) != 0) {
		y1 = xy.y - (xy.y % 100);
		y2 = xy.y + (100 - (xy.y % 100));
	}
	else {
		y1 = xy.y;
		y2 = xy.y + 100;
	}

	//combine the four sides to make the four corners
	Point topleft(x1,y1); Point botright(x2,y2);
	Point topright(x2,y1); Point botleft(x1,y2);
	vector<Point2f> corners;
	corners.push_back(topleft);	corners.push_back(topright);
	corners.push_back(botleft);	corners.push_back(botright);

	return corners;
}













