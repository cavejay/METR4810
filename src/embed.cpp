/*
 * embed.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Jonathan
 */

#include <Python.h>
#include "embed.h"
#include <iostream>
using namespace std;

PyObject* pyConn(void) {
	// Initialize the Python Interpreter
	Py_Initialize();


	PyObject *pName, *pModule, *pDict, *pFunc, *pSerial;

	// Build the name object
	pName = PyString_FromString("bthConn");

	// Load the module object
	pModule = PyImport_Import(pName);

	// pDict is a borrowed reference
	pDict = PyModule_GetDict(pModule);

	// pFunc is also a borrowed reference
	pFunc = PyDict_GetItemString(pDict, "blue");
	Py_INCREF(pFunc);

	if (PyCallable_Check(pFunc))
	{
		pSerial = PyObject_CallObject(pFunc, NULL);
	} else
	{
		PyErr_Print();
		// Clean up
	    Py_DECREF(pModule);
	    Py_DECREF(pName);
	    Py_DECREF(pFunc);
	    Py_Finalize();
	}

	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_DECREF(pFunc);

	// Don't finalise this section so that the reference is kept to an instance of the class?
	// This may allow a class versiont to be used if passing *PyObject doesn't work
	//Py_Finalize();

	return pSerial;
}


int sendPy(PyObject *serial,int mlr, int mfb) {
		cout<< mlr;
		cout<< mfb;
	// Initialize the Python Interpreter
	    //Py_Initialize();

	    PyObject *pName, *pModule, *pDict, *pFunc;
	    PyObject *pArgs, *pValue;

	    // Build the name object
	    pName = PyString_FromString("bthConn");

	    // Load the module object
	    pModule = PyImport_Import(pName);

	    // pDict is a borrowed reference
	    pDict = PyModule_GetDict(pModule);

	    // pFunc is also a borrowed reference
	    pFunc = PyDict_GetItemString(pDict, "send");
	    Py_INCREF(pFunc);

	    int args[2] = {mlr,mfb};
	    pArgs = PyTuple_New(3);
	    for (int i = -1; i < 2; i++) {

	        /* pValue reference stolen here: */

	        // For first reference, add PyObject service to tuple
	        if(i == -1) {
	        	PyTuple_SetItem(pArgs, i+1, serial);
	        }else {
	        	pValue = PyLong_FromLong(args[i]);
	        	if (!pValue) {
	        		        	Py_DECREF(pArgs);
	        		            Py_DECREF(pModule);
	        		            Py_DECREF(pValue);
	        		            Py_DECREF(pName);
	        		            fprintf(stderr, "Cannot convert argument\n");
	        		            return 1;
	        		        }
	        	PyTuple_SetItem(pArgs, i+1, pValue);
	        }

	    }
	    cout << pArgs;
	    pValue = PyObject_CallObject(pFunc, pArgs);
	    Py_DECREF(pArgs);
	    if (pValue != NULL) {
	    	printf("Result of call: %ld\n", PyLong_AsLong(pValue));
	        Py_DECREF(pValue);
	    }
	    else {
	    	Py_DECREF(pFunc);
	        Py_DECREF(pModule);
	        Py_DECREF(pValue);
	        Py_DECREF(pName);
	        PyErr_Print();
	        fprintf(stderr,"Call failed\n");
	        return 1;
	    }

	    // Clean up
	    Py_DECREF(pModule);
	    Py_DECREF(pName);
	    Py_DECREF(pFunc);
	    // Finish the Python Interpreter

	    // Join thread and close off Py_Finalize() in a final function
	    //Py_Finalize();

	    return 0;

}

void pyDc(void) {
	// Use this function to close off the python interpreter
	Py_Finalize();
}
