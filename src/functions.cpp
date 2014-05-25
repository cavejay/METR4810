/*
 * functions.cpp
 *
 *  Created on: 28/03/2014
 *      Author: Michael
 */
#include "functions.h"


inputVars getInputData(int argc, char* argv[])
{
  inputVars toReturn;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
	show_usage(argv[0]);
	toReturn.varsParsedSuccessfully = false;

    } else if ((arg == "-f") || (arg == "--file")) {
   	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
   	    toReturn.loadFile = true;
   	    toReturn.filename = string(argv[i++]); // Increment 'i' so we don't get the argument as the next argv[i].
   	    return toReturn;
   	} else { // Uh-oh, there was no argument to the destination option.
   	      std::cerr << "--destination option requires one argument." << std::endl;
   	      toReturn.varsParsedSuccessfully = false;
   	}

    } else if ((arg == "-s") || (arg == "--inputsource")) {
	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
	    toReturn.inputSource = string(argv[i++]); // Increment 'i' so we don't get the argument as the next argv[i].
	} else { // Uh-oh, there was no argument to the destination option.
	      std::cerr << "--destination option requires one argument." << std::endl;
	      toReturn.varsParsedSuccessfully = false;
	}

    } else if ((arg == "-H") || (arg == "--host")) {
    	if (i + 1 < argc) {
    	    toReturn.Host = argv[i++];
    	} else {
    	      std::cerr << "--host option requires one argument." << std::endl;
    	      toReturn.varsParsedSuccessfully = false;
    	}

    } else if ((arg == "-d") || (arg == "--destination")) {
    	if (i + 1 < argc) {
    	    toReturn.file_location = string(argv[i++]);
    	} else {
    	      std::cerr << "--destination option requires one argument." << std::endl;
    	      toReturn.varsParsedSuccessfully = false;
    	}

    } else if ((arg == "-c") || (arg == "--cameranumber")) {
	if (i + 1 < argc) {
	    toReturn.camera_number = atoi(argv[i++]);
	} else {
	      std::cerr << "--cameranumber option requires one argument." << std::endl;
	      toReturn.varsParsedSuccessfully = false;
	}

    } else if ((arg == "-mt") || (arg == "--multithreading")) {
    	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
    	    toReturn.multithreading = true; // Increment 'i' so we don't get the argument as the next argv[i].
    	}

    } else if ((arg == "-sim") || (arg == "--simulation")) {
  	if (i + 1 < argc) {
  	    toReturn.Simulation = true;
  	}

    } else if ((arg == "-demo") || (arg == "--demonstration")) {
      	if (i + 1 < argc) {
      	    toReturn.demo_mode = true;
      	}

    } else {
      cout << "Invalid input, this program must be run with commands" << endl;
      show_usage(argv[0]);
      toReturn.varsParsedSuccessfully = false;
    }
  }
  return toReturn;
}


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


void Dilation(const Mat& src, Mat& dst, int dilation_shape, double dilation_size)
{
  int dilation_type;
  if( dilation_shape == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_shape == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_shape == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  cv::dilate( src, dst, element);
}


void Erosion(const Mat& src, Mat& dst, int erosion_shape, double erosion_size)
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

int getSearchRadius(int preferedPoints, Point2f carCenter, vector<Point> outside, vector<Point> inside)
{
  int value = 0;
  int totalPoints = 0;
  int circRadius = 0;
  while(totalPoints < preferedPoints){
    totalPoints = 0;
    int Points1 = 0;
    int Points2 = 0;

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















