/*
 * PID.h
 *
 *  Created on: 14/04/2014
 *      Author: Michael
 */

#ifndef PID_H_
#define PID_H_

#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class PID
{
private:
  bool Using [3];
  double pval;
  vector<float> error_list;

public:
  PID ();
  void P(double Proportional);
  void I();
  void D();
  void Er(float Error_Value);
};

#endif /* PID_H_ */
