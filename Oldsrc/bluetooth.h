/*
 * bluetooth.h
 *
 *  Created on: 28/05/2014
 *      Author: Jonathan
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <windef.h>
#include <windows.h>
#include <WinSock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <bluetoothapis.h>
#include "jonohax.h"
#include <iostream>
#include <initguid.h>

#define ARRAY_LENGTH(array) (sizeof(array)/sizeof((array)[0]))

int blue(void);




#endif /* BLUETOOTH_H_ */
