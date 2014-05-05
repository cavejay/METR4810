/*
 * PID.h
 *
 *  Created on: 05/05/2014
 *      Author: Michael
 */

#ifndef PID_H_
#define PID_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class PID
{
  float pConstant;
  float iConstant;
  float dConstant;

  vector<float> error_list;

  int integrationLen;

public:
  // Constructor
  PID (float pConstant, float iConstant, float dConstant, int integrationLen = 10);

  // Return Control constants
  float Kp(void);
  float Ki(void);
  float Kd(void);

  // Set Control constants
  void Kp(float pConstant);
  void Ki(float iConstant);
  void Kd(float dConstant);

  // Get/Set latest error value
  void error(float error);
  float error(void);

  // Controller returns
  float p();
  float i();
  float d();
  float p_i();
  float p_d();
  float p_i_d();
  float i_d();

  // Set integration Length
  void set_integrationLen(float integrationLen);
};

#endif /* PID_H_ */
