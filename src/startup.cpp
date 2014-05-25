/*
 * startup.cpp
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */
#include "startup.h"

inputVars getInputData(int argc, char* argv[])
{
  inputVars toReturn;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
	show_usage(argv[0]);
	toReturn.varsParsedSuccessfully = false;

    } else if ((arg == "-f") || (arg == "--file")) {
   	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
   	    toReturn.loadFile = true;
   	    toReturn.filename = string(argv[i++]); // Increment 'i' so we don't get the argument as the next argv[i].
   	    return toReturn;
   	} else { // Uh-oh, there was no argument to the destination option.
   	      std::cerr << "--destination option requires one argument." << std::endl;
   	      toReturn.varsParsedSuccessfully = false;
   	}

    } else if ((arg == "-g") || (arg == "--generatefile")) {


	toReturn.varsParsedSuccessfully = false;
	return toReturn;

    } else if ((arg == "-s") || (arg == "--inputsource")) {
	if (i + 1 < argc) { // Make sure we aren't at the end of argv!
	    toReturn.inputSource = string(argv[i++]); // Increment 'i' so we don't get the argument as the next argv[i].
	} else { // Uh-oh, there was no argument to the destination option.
	      std::cerr << "--destination option requires one argument." << std::endl;
	      toReturn.varsParsedSuccessfully = false;
	}

    } else if ((arg == "-H") || (arg == "--host")) {
    	if (i + 1 < argc) {
    	    toReturn.Host = argv[i++];
    	} else {
    	      std::cerr << "--host option requires one argument." << std::endl;
    	      toReturn.varsParsedSuccessfully = false;
    	}

    } else if ((arg == "-d") || (arg == "--destination")) {
    	if (i + 1 < argc) {
    	    toReturn.file_location = string(argv[i++]);
    	} else {
    	      std::cerr << "--destination option requires one argument." << std::endl;
    	      toReturn.varsParsedSuccessfully = false;
    	}

    } else if ((arg == "-c") || (arg == "--cameranumber")) {
	if (i + 1 < argc) {
	    toReturn.camera_number = atoi(argv[i++]);
	} else {
	      std::cerr << "--cameranumber option requires one argument." << std::endl;
	      toReturn.varsParsedSuccessfully = false;
	}

    } else if ((arg == "-sim") || (arg == "--simulation")) {
	toReturn.Simulation = true;

    } else if ((arg == "-demo") || (arg == "--demonstration")) {
	toReturn.demo_mode = true;

    } else {
      cout << "Invalid input, this program must be run with commands" << endl;
      show_usage(argv[0]);
      toReturn.varsParsedSuccessfully = false;
    }
  }
  return toReturn;
}

void createSettingsFile(void){

}

inputVars readSettingsFile(String file){
  return 0;
}


