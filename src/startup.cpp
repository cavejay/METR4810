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
   	      std::cerr << "--file option requires one argument." << std::endl;
   	      toReturn.varsParsedSuccessfully = false;
   	}

    } else if ((arg == "-g") || (arg == "--generatefile")) {
	createSettingsFile();
	cerr << "Settings file created successfully, please run again with -f <filename>" << endl;
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
    	    toReturn.host = argv[i++];
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
	    toReturn.cameraID = atoi(argv[i++]);
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
  fs << "Input Settings" << "Read the included readme.md for more information about these settings";
  fs << "Input Format" << "roborealm";
  fs << "RoboRealm Host Address" << "localhost";
  fs << "First port" << 6060;
  fs << "Number of cameras connected" << 4;

  fs << "File Location" << "Sample_Pictures/demo-track.png";

  fs << "Device ID of the camera connected" << 0;

  fs << "Advanced Settings" << "For deeper changes of the program" ;
  fs << "Show debug images and print outs" << false;
  fs.release();
}

inputVars readSettingsFile(String file){
  inputVars toReturn;
  // Check if the file can actually be opened
  cout << "Opening settings file" << endl;
  try {
	  FileStorage fs(file, FileStorage::READ);
	  // Read in dem vars
	  fs["Input Format"] >> toReturn.inputSource;
	  String tempHost;
	  fs["RoboRealm Host Address"] >> tempHost;
	  toReturn.host = new char[tempHost.length() + 1];
	  strcpy(toReturn.host,tempHost.c_str());

	  fs["First Port"] >> toReturn.ports;
	  fs["Number of cameras connected"] >> toReturn.numCameras;
	  fs["File Location"] >> toReturn.file_location;
	  fs["Device ID of the camera connected"] >> toReturn.cameraID;
	  fs["Show debug images and print outs"] >> toReturn.showWorking;
	  cout << "Returning variables" << endl;
  } catch (...){
	  cerr << "Settings file could not be opened.\n"
			  "Please check and ensure it ends in .yml" << endl;
  }
  return toReturn;
}

/**
 * Needs to take an image of the starting tile
 * and return a integer that shows what stage the starting lights are at
 * 3 = 1 red
 * 2 = 2 red
 * 1 = green and 2 red and thus start
 */
int startingLights(const Mat& src){
  // TODO make a function to find the starting lights

	return 0;
}




















