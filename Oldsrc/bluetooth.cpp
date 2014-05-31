
#include "bluetooth.h"
const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
DEFINE_GUID(GUID_BTH, 0xE0CBF06C, 0xCD8B, 0x4647, 0xBB, 0x8A, 0x26, 0x3B, 0x43, 0xF0, 0xF9, 0x74);
// e0cbf06c-cd8b-4647-bb8a-263b43f0f974 to hex

int blue(void) // int argc, char* argv[]
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	GUID* SCId;
	SOCKET_ADDRESS remAD;
	SOCKET_ADDRESS locAD;


	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
	    /* Tell the user that we could not find a usable */
	    /* Winsock DLL.                                  */
		std::cout << "Error using winsock DLL\n";
	    return 1;
	}

	//Initialising query for device
	WSAQUERYSET queryset;
	memset(&queryset, 0, sizeof(WSAQUERYSET));
	queryset.dwSize = sizeof(WSAQUERYSET);
	queryset.dwNameSpace = NS_BTH;

	HANDLE hLookup;
	INT result;
	result = WSALookupServiceBegin(&queryset, LUP_CONTAINERS, &hLookup);
	if (result!=0){
	    std::cout << "An error occured while initialising look for devices, closing....";
	exit(result);
	}

	//Initialisation succeed, start looking for devices
	BYTE buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	DWORD bufferLength = sizeof(buffer);
	WSAQUERYSET *pResults = (WSAQUERYSET*)&buffer;
	WSAQUERYSET *qResults = NULL;
	while (result==0){
	    result = WSALookupServiceNext(hLookup, LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_TYPE | LUP_RETURN_BLOB | LUP_RES_SERVICE, &bufferLength, pResults);
	    if(result==0){// A device found
	        //print the name of the device
	        LPTSTR s = pResults->lpszServiceInstanceName;
	        GUID* sid = pResults->lpServiceClassId;
	        CSADDR_INFO* addr = pResults->lpcsaBuffer;
	        std::cout << s << " found. \n";
	        std::cout << &sid << " is the service class Id. \n";
	        std::cout << &addr << " is the buffer of SOCKADDR_BTH with address inside. \n";
	        remAD = addr->RemoteAddr;
	        //std::cout<<remAD;
	        //std::cout << (pResults->lpNSProviderId->Data4);
	        //std::cout << (pResults->lpszComment);
	        //std::cout << (pResults->lpszContext);

	        if(s=="HC-05") {
	        	SCId = sid;
	        	remAD = addr->RemoteAddr;
	        	locAD = addr->LocalAddr;
	        	qResults = pResults;
	        }
//	        //Initialise quering the device services
//	        WSAQUERYSET queryset2;
//	        memset(&queryset2, 0, sizeof(queryset2));
//	        queryset2.dwSize = sizeof(queryset2);
//	        queryset2.dwNameSpace = NS_BTH;
//	        queryset2.dwNumberOfCsAddrs = 0;
//	        CSADDR_INFO * addr = (CSADDR_INFO *)pResults->lpcsaBuffer;
//	        char* addressAsString[1000];
//	        DWORD addressSize = sizeof(addressAsString);
//	        WSAAddressToString(addr->RemoteAddr.lpSockaddr,addr->RemoteAddr.iSockaddrLength,NULL,addressAsString, &addressSize);
//	        queryset2.lpszContext = addressAsString;
//	        GUID protocol = L2CAP_PROTOCOL_UUID;
//	        queryset2.lpServiceClassId = &protocol;
//	        HANDLE hLookup2;
//	        INT result2;
//	        result2 = WSALookupServiceBegin(&queryset2, LUP_FLUSHCACHE |LUP_RETURN_NAME | |LUP_RETURN_TYPE| LUP_RETURN_BLOB | LUP_RETURN_COMMENT, &hLookup2);
//	        if (result2 !=0){
//	        	std::cout << "An error occured while initializing query for services";
//	            exit(result);
//	        }
//
//	        //Start quering for device services
//	        while(result2 ==0){
//	        	BYTE buffer2[4096];
//	            memset(buffer2, 0, sizeof(buffer2));
//	            DWORD bufferLength2 = sizeof(buffer2);
//	            WSAQUERYSET *pResults2 = (WSAQUERYSET*)&buffer2;
//	            result2 = WSALookupServiceNext(hLookup2,LUP_FLUSHCACHE |LUP_RETURN_NAME | LUP_RETURN_TYPE | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RETURN_COMMENT,&bufferLength2,pResults2);
//	            if(result2 == 0) {
//	            	std::cout << "Service found: " << pResults2->lpszServiceInstanceName <<"\n";
//	            }
//	        }
	    }
	}







	SOCKADDR_BTH sockAddr;
	SOCKET blueSocket;

	blueSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	memset (&sockAddr, 0, sizeof(sockAddr));
	WSASetService(qResults, RNRSERVICE_REGISTER, 0);
	//NameToBthAddr("HC-05", &sockAddr);
	sockAddr.addressFamily = AF_BTH;
	sockAddr.port = BT_PORT_ANY; // May need to change this if the channel interferes with another team
	//sockAddr.serviceClassId = GUID_BTH;
	sockAddr.btAddr = 12345205070; // Value in octal?
	//sockAddr.btAddr = 2205445103152740; // Nexus 5
	std::cout<<GET_NAP(sockAddr.btAddr)<<" \n";
	std::cout<<GET_SAP(sockAddr.btAddr)<<" \n";
	std::cout<<SET_NAP_SAP(GET_NAP(sockAddr.btAddr), GET_SAP(sockAddr.btAddr))<<" \n";

	//sockAddr.port = BT_PORT_ANY; // May need to change this if the channel interferes with another team
	//sockAddr.btAddr = // Need to pair devices to then check for this
	//bind(,sockAddr,);
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
