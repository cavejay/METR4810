/*
 * embed.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Jonathan
 */

#include <Python.h>

int main() {

	// Initialize the Python Interpreter
	    Py_Initialize();

	    PyObject *pName, *pModule, *pDict, *pFunc, *pValue;

	    // Build the name object
	    pName = PyString_FromString("test1");

	    // Load the module object
	    pModule = PyImport_Import(pName);

	    // pDict is a borrowed reference
	    pDict = PyModule_GetDict(pModule);

	    // pFunc is also a borrowed reference
	    pFunc = PyDict_GetItemString(pDict, "testFunction");

	    if (PyCallable_Check(pFunc))
	    {
	        PyObject_CallObject(pFunc, NULL);
	    } else
	    {
	        PyErr_Print();
	    }

	    // Clean up
	    Py_DECREF(pModule);
	    Py_DECREF(pName);

	    // Finish the Python Interpreter
	    Py_Finalize();




}


