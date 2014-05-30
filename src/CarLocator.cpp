/*
 * Locator.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */

#include "CarLocator.h"

CarLocator::CarLocator ()
{
  // TODO figure out if anything actually needs to go here?
}

Point CarLocator::findCar(const Mat& src){
  newIMG(src);
  Point rawr;
  return rawr;
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
