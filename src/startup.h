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
  bool varsParsedSuccessfully = true;
  bool loadFile = false;
  String filename = "METR4810_Settings.yml";
  String inputSource = "still";
  char* Host = "127.0.0.1";
  String file_location = "Sample_Pictures/demo-track.png";
  int camera_number = 0;
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
