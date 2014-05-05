/*
 * PId.cpp
 *
 *  Created on: 05/05/2014
 *      Author: Michael
 */

#include "PID.h"

PID::PID (float pConstant, float iConstant, float dConstant, int integrationLen)
{
  this->pConstant = pConstant;
  this->iConstant = iConstant;
  this->dConstant = dConstant;
  this->integrationLen = integrationLen;
}

// Proportional Controller
float PID::p(){
  return this->error()*pConstant;
}

// Integral Controller
float PID::i(){
  float integrate = 0;
  if(error_list.size() > integrationLen)
  {
    for(size_t i = error_list.size()-(integrationLen+1); i < error_list.size()-1; i++)
    {
	integrate += (error_list[i] + error_list[i+1])/2;
    }
    std::cout << "integrate: " << integrate << std::endl;
  }
  return integrate*iConstant;
}

// Derivative controller
float PID::d(){
  float errorDifference = error_list[error_list.size()-1] - error_list[error_list.size()-2];
  return errorDifference*dConstant;
}

float PID::p_i(){
  return p() + i();
}

float PID::p_d(){
  return p() + d();
}

float PID::p_i_d(){
  return p() + i() + d();
}

float PID::i_d(){
  return i() + d();
}

float PID::Kp(void){
  return pConstant;
}

float PID::Ki(void){
  return iConstant;
}

float PID::Kd(void){
  return dConstant;
}

void PID::Kp(float pConstant){
  this->pConstant = pConstant;
}

void PID::Ki(float iConstant){
  this->iConstant = iConstant;
}

void PID::Kd(float dConstant){
  this->dConstant = dConstant;
}

void PID::error(float error){
  error_list.push_back(error);
}

float PID::error(void){
  return error_list[error_list.size()-1];
}

void PID::set_integrationLen(float integrationLen){
  this->integrationLen = integrationLen;
}
