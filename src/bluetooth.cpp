#include <windef.h>
#include <windows.h>

#include <WinSock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <bluetoothapis.h>
#include "jonohax.h"

int bluetoothConn (void) // int argc, char* argv[]
{
	WSADATA wsd;
	WSAStartup (MAKEWORD(1,0), &wsd);

	SOCKADDR_BTH sockAddr;
	SOCKET blueSocket;
	int err;

	blueSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	memset (&sockAddr, 0, sizeof(sockAddr));
	sockAddr.addressFamily = AF_BTH;
	sockAddr.port = BT_PORT_ANY; // May need to change this if the channel interferes with another team
	//sockAddr.btAddr = // Need to pair devices to then check for this
	err = connect(blueSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));


	if(err!=0) {
		// An error has occurred when connecting
		return -1;
	}
	return 0;
}
