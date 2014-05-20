/*
 * sys_constants.h
 *
 *  Created on: 19/05/2014
 *      Author: Jonathan
 */

#ifndef SYS_CONSTANTS_H_
#define SYS_CONSTANTS_H_

#define _FLOAT_H___

#define FLT_MAX		__FLT_MAX__
#define DBL_MAX		__DBL_MAX__

#include <cfloat>
#include <limits>

#define DBL_EPSILON std::numeric_limits<double>::epsilon()
#define FLT_EPSILON 1.19209290E-07F // decimal constant

#endif /* SYS_CONSTANTS_H_ */
