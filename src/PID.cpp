/*
 * PID.cpp
 *
 *  Created on: 14/04/2014
 *      Author: Michael
 */

#include "PID.h"

PID::PID ()
{
  for(int i = 0; i < 3;i++) {
    Using[i] = false;
  }

}

void PID::P(double Proportional)
{
  Using[0] = true;
  this->pval = Proportional;
}

void PID::I()
{
  Using[1] = true;
}

void PID::D()
{
  Using[3] = true;
}

void PID::Er(float Error_Value)
{
  error_list.push_back(Error_Value);
}


