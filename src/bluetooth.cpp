#include <winsock2.h>
#include <ws2bth.h>

SOCKADDR_BTH sockAddr;
SOCKET blueSocket;
int err;

blueSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
memset (&sockAddr, 0, sizeof(sockAddr));
sockAddr.addressFamily = AF_BTH;
sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID; // Default windows protocol
sockAddr.port = BT_PORT_ANY; // May need to change this if the channel interferes with another team
sockAddr.blueAddr = <your bluetooth address> // Need to pair devices to then check for this
err = connect(blueSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));

if(error!=0) {
	// An error has occurred when connecting
}