///*
// * embedPython.cpp
// *
// *  Created on: 19/05/2014
// *      Author: Jonathan
// */
//#include <Python.h>
//
//int embedPy() {
//
//	// Initialize the Python Interpreter
//	    Py_Initialize();
//
//	    // Build the name object
//	    pName = PyString_FromString(argv[1]);
//
//	    // Load the module object
//	    pModule = PyImport_Import(pName);
//
//	    // pDict is a borrowed reference
//	    pDict = PyModule_GetDict(pModule);
//
//	    // pFunc is also a borrowed reference
//	    pFunc = PyDict_GetItemString(pDict, argv[2]);
//
//	    if (PyCallable_Check(pFunc))
//	    {
//	        PyObject_CallObject(pFunc, NULL);
//	    } else
//	    {
//	        PyErr_Print();
//	    }
//
//	    // Clean up
//	    Py_DECREF(pModule);
//	    Py_DECREF(pName);
//
//	    // Finish the Python Interpreter
//	    Py_Finalize();
//
//
//
//
//}
