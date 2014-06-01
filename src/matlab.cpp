/*
 * matlab.cpp
 *
 *  Created on: 31/05/2014
 *      Author: Jonathan
 */


/*
 * matlab.cpp
 *
 * Connecting to bluetooth via the Matlab engine
 *
 *  Created on: 30/05/2014
 *      Author: Jonathan
 */
// Put variable example = (first arg: engine, second arg: name of var, third arg: mx variable)
// engPutVariable(ep, "T", T);

#include "matlab.h"

Engine* matConnBlue()

{
	Engine *ep;


	/*
	 * Call engOpen with a NULL string. This starts a MATLAB process
     * on the current host using the command "matlab".
	 */
	if (!(ep = engOpen(""))) {
		std::cout << "\nCan't start MATLAB engine\n";
	}

	// Once the engine has opened, create the bluetooth object
	engEvalString(ep, "object = Bluetooth('HC-05', 1);");

	// Open the bluetooth file stream to later send data across
	engEvalString(ep, "fopen(object);");

	// Set the baud rate to 9600
	//engEvalString(ep, "set(object,'BaudRate',9600);");

	// Open the bluetooth file stream to later send data across
	engEvalString(ep, "fopen(object);");

	std::string msg = "1";
	mxArray *mx;
	const char* send = msg.c_str();
	if((mx = mxCreateString(send)) == NULL ) {
		std::cout << "Unable to convert message to mxArray\n";
	} else {
		if(engPutVariable(ep, "msg", mx)) {
			std::cout << "Unable to put mx into engine workspace\n";
		}
		mxDestroyArray(mx);
	}

	engEvalString(ep, "fwrite(object, msg);");

	return ep;
}

void matSend(Engine *ep, std::string msg) {

	mxArray *mx;
	const char* send = msg.c_str();
	if((mx = mxCreateString(send)) == NULL ) {
		std::cout<<"Unable to convert message to mxArray\n";
	    return;
	} else {
		if(engPutVariable(ep, "msg", mx)) {
			printf("Unable to put t2 into engine workspace\n");
		std::cout << "Unable to convert message to mxArray\n";
	    return;
	} else {
		if(engPutVariable(ep, "msg", mx)) {
			std::cout << "Unable to put mx into engine workspace\n";
		}
		mxDestroyArray(mx);
	}

	engEvalString(ep, "fwrite(object, msg);");
	engEvalString(ep, "x = uint8(str2num(msg));");
	engEvalString(ep, "fwrite(object, x);");
}


int matDiscBlue(Engine *ep) {

	// Clear matlab variables created during bluetooth connection
	engEvalString(ep, "fwrite(object, 127);");
	engEvalString(ep, "fwrite(object, 127);");
	engEvalString(ep, "delete(object);");
	engEvalString(ep, "clearvars;");
	engEvalString(ep, "clear;");
	engClose(ep);

	return 0;
}


