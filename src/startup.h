/*
 * startup.h
 *
 *  Created on: 25/05/2014
 *      Author: Michael
 */

#ifndef STARTUP_H_
#define STARTUP_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "C++/MinGW/RR_API.h"

using namespace cv;
using namespace std;

// Structs
struct inputVars {
	// init variables
  bool varsParsedSuccessfully = true;
  bool loadFile = false;

  // VStream
  String filename = "METR4810_Settings.yml";
  String inputSource = "still";
  char* host = "127.0.0.1";
  int ports = 6060;
  String file_location = "Sample_Pictures/demo-track.png";
  int cameraID = 0;
  int numCameras = 4;

  // Running modes
  bool multithreading = false;
  bool Simulation = false;
  bool demo_mode = false;
  bool showWorking = false;
};

// FUNCTIONS

inputVars getInputData(int argc, char* argv[]);
void createSettingsFile(void);
inputVars readSettingsFile(String file);

int startingLights(const Mat& src);


#endif /* STARTUP_H_ */
