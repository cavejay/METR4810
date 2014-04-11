/*
 * RobotSim.cpp
 *
 *  Created on: 11/04/2014
 *      Author: Michael
 */

#include "RobotSim.h"

RobotSim::RobotSim(Point2d Position, float angle, String name, Size R_size)
{
  this->angle = angle;
  this->Position = Position;
  this->R_size = R_size;
  if(!(name == "")){this->name = name;}
    else {this->name = "Robot_Name";}

}

  /*
   * Draws the "robot" as a triangle faceing in the
   * direction it's initialised as.
   */
void RobotSim::draw(Mat& src)
{
  cv::RotatedRect Rekt = RotatedRect(Position, R_size, angle);
  Point2f vertices[4];
  Rekt.points(vertices);
  Point2f Front((vertices[2].x+vertices[3].x)/2, (vertices[2].y+vertices[3].y)/2);
  cv::line(src, vertices[0], Front, Scalar(255,0,0));
  cv::line(src, vertices[1], Front, Scalar(255,0,0));
  cv::line(src, vertices[0], vertices[1], Scalar(255,0,0));
}


/*
 * Moves the Robot a certain distance 'dist' in the direction of angle
 * in respect to it's current anglular direction. *
 */
void RobotSim::move(int dist, float angle)
{
  Position.x += dist * cos((angle+this->angle)* PI_NUM /180);
  Position.y += dist * sin((angle+this->angle)* PI_NUM /180);
  this->angle += angle;
}
