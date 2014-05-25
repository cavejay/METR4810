/*
 * RobotSim.cpp
 *
 *  Created on: 11/04/2014
 *      Author: Michael
 */

#include "RobotSim.h"

RobotSim::RobotSim(Point2d Position, float angle, String name, float MAXturningSpeed, Size R_size)
{
  this->angle = angle;
  this->Position = Position;
  this->R_size = R_size;
  this->MAXturningSpeed = MAXturningSpeed;
  if(!(name == "")){this->name = name;}
    else {this->name = "Robot_Name";}

  // init other values
  searchRadius = 0;
  turningStack = 0;
  cantakenewAngle = true;
}
/*
 * Makes the search Radius shown by the simulation equal a certain value
 * This does not change the actual pathing, merely the presentation of said
 * pathing.
 */
void RobotSim::set_searchRadius(int searchRadius)
{
  this->searchRadius = searchRadius;
}

  /*
   * Draws the "robot" as a triangle facing in the
   * direction it's initialised as.
   */
void RobotSim::draw(Mat& src, bool draw_searchRadius)
{
  cv::RotatedRect Rekt = RotatedRect(Position, R_size, angle);
  Point2f vertices[4];
  Rekt.points(vertices);
  Point2f Front((vertices[2].x+vertices[3].x)/2, (vertices[2].y+vertices[3].y)/2);
  cv::line(src, vertices[0], Front, Scalar(255,0,0));
  cv::line(src, vertices[1], Front, Scalar(255,0,0));
  cv::line(src, vertices[0], vertices[1], Scalar(255,0,0));
  if(draw_searchRadius){
    circle(src, Position, searchRadius, Scalar(0,0,255), 2, 8, 0 );
  }
}

void RobotSim::drawTrace(Mat& src)
{
	for(int i = 0; i < History.size(); i++){
		circle(src, History[i], 1, Scalar(234,0,255), -1);
	}
}

void RobotSim::showDirection(Mat& src, float value1, float value2)
{
  float a = (value1 - value2)/(value1);
  // draw the panel to show that we're turning right
  cout << "dirction percent hting " << a << endl;
  // Make the rectangles
  Point rightPoint1 = Point(src.size().width-30,30);
  Point rightPoint2 = Point(rightPoint1.x - 40, rightPoint1.y+100);
  Point leftPoint1 = Point(rightPoint2.x - 10, rightPoint2.y);
  Point leftPoint2 = Point(leftPoint1.x - 40 , leftPoint1.y-100);

  //only draw certain rectangles
  if(a < 0){
    a = abs(a);
    rectangle(src,rightPoint1, rightPoint2,Scalar(0,255*a,0), CV_FILLED);
  } else {
    rectangle(src,leftPoint1, leftPoint2,Scalar(0,0,255*a), CV_FILLED);
  }
}

/*
 * Moves the Robot a certain distance 'dist' in the direction of angle
 * in respect to its current angular direction.
 */
void RobotSim::move(float dist, float angle)
{
  History.push_back(Position);
  Position.x += dist * cos((angle+this->angle)* PI_NUM /180);
  Position.y += dist * sin((angle+this->angle)* PI_NUM /180);
  this->angle += angle;
}
/*
 * 'drives' the Robot at a certain speed in a certain direction in respect
 * to its current angular direction.
 * Angles will only be played after the previous angle has been turned.
 */
void RobotSim::drive(float velocity, float angle, float turningSpeed)
{
  // Add the angle to the stack
  if(cantakenewAngle == true){
    turningStack = angle;
    cantakenewAngle = false;
  } else {
    cout << "Currently turning, " << turningStack << "degrees remaining\n";
  }

  // subtract the angle we're turning from the turning stack and then make the turn
  if(turningStack < 0) {
    turningStack += turningSpeed;
  } else if (turningStack > 0) {
    turningStack -= turningSpeed;
  }
  float angle2Turn = turningSpeed;
  this->move(velocity,angle2Turn);

  if (turningStack <= 2*turningSpeed && turningStack >= -2*turningSpeed){
    cantakenewAngle = true;
  }

}
