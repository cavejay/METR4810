/*
 * embed.h
 *
 *  Created on: 26/05/2014
 *      Author: Jonathan
 */

#ifndef EMBED_H_
#define EMBED_H_

#include <Python.h>
PyObject* pyConn(void);
int sendPy(PyObject* service,int mlr, int mfb);
void pyDc(void);


#endif /* EMBED_H_ */
