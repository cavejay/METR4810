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
	createSettingsFile();
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

/**
 * Run this if you've lost the settings file, or want to reset to default.
 */
void createSettingsFile(void){

  // Create an example file with all the default settings
  FileStorage fs("METR4810_Settings.yml", FileStorage::WRITE);
  fs << "Input Format" << "roborealm";
  fs << "RoboRealm Host Address" << "localhost";
  fs << "File Location" << "Sample_Pictures/demo-track.png";
  fs.release();
}

inputVars readSettingsFile(String file){
  // TODO create the read from yml function.
  inputVars toReturn;
  // Check if the file can actually be opened
  if(!(file.substr(file.size()-5,file.size()-1) == ".yml")){
    toReturn.varsParsedSuccessfully;
    cerr << "Settings file did not end in '.yml'" << endl;
    return toReturn;
  }

  FileStorage fs(file, FileStorage::READ);

  // Read in dem vars
  fs["Input Format"] >> toReturn.inputSource;
  String tempHost;
  fs["RoboRealm Host Address"] >> tempHost;
  strcpy(toReturn.Host,tempHost.c_str());
  fs["File Location"] >> toReturn.file_location;
  return toReturn;
}

int startingLights(const Mat& src){
  // TODO make a function to find the starting lights
  return 0;
}




















