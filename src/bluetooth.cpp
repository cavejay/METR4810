#include <windef.h>
#include <windows.h>

#include <WinSock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <bluetoothapis.h>
#include "jonohax.h"
#include <iostream>

#define ARRAY_LENGTH(array) (sizeof(array)/sizeof((array)[0]))

int wut(void) // int argc, char* argv[]
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
	    /* Tell the user that we could not find a usable */
	    /* Winsock DLL.                                  */
		std::cout << "Error using winsock DLL\n";
	    return 1;
	}

	SOCKADDR_BTH sockAddr;
	SOCKET blueSocket;

	blueSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	memset (&sockAddr, 0, sizeof(sockAddr));
	sockAddr.addressFamily = AF_BTH;
	sockAddr.port = 4; //BT_PORT_ANY; // May need to change this if the channel interferes with another team
	//sockAddr.serviceClassId = {00000000-0000-0000-0000-000000000000};
	sockAddr.btAddr = 12345205070; // Value in octal?
//	sockAddr.serviceClassId = 0x1101; // Default windows protocol
	sockAddr.port = BT_PORT_ANY; // May need to change this if the channel interferes with another team
	//sockAddr.btAddr = // Need to pair devices to then check for this
	err = connect(blueSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));


	if(err!=0) {
		// An error has occurred when connecting
		std::cout << "Error connecting socket\n";
		std::cout << WSAGetLastError();
		return 2;
	}

	const char buff[1] = {1};
	const char *ptrToBuff = buff;
	size_t length = ARRAY_LENGTH(buff);
	send(blueSocket, ptrToBuff, length, 0);
	return 0;
}
