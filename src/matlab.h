/*
 * matlab.h
 *
 *  Created on: 31/05/2014
 *      Author: Jonathan
 */

#ifndef MATLAB_H_
#define MATLAB_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "engine.h"

Engine* matConnBlue();
void matSend(Engine *ep, std::string msg);
int matDiscBlue(Engine *ep);

#endif /* MATLAB_H_ */
