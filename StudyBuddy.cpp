#include <iostream>
#include "WinSock2.h"
#include "WS2tcpip.h"
#include <string.h>
using std::cout;
using std::endl;
using std::cin;
using std::string;


// Adam Loving and Carter Fitch

#pragma comment(lib, "Ws2_32.lib")
const int DEFAULT_BUFLEN = 512;

//code provided
int wait(SOCKET s, int seconds, int msec) {
	// Parameter List:
	// [IN] s        : SOCKET handle of connected socket.
	// [IN] seconds : Number of seconds to wait for incoming traffic
	// [IN] msec    : Number of milliseconds to wait
	// Return value : 1 = Data is waiting in TCP/UDP buffer; 0 = Timeout or error detected
	int    stat;                     // Status of waiting socket
	struct timeval stTimeOut;        // Used to set timeout value
	fd_set stReadFDS;                // "File Descriptor Set" for select() call
	fd_set stXcptFDS;                // "File Descriptor Set" for exception handling

	// Set TimeOut value
	stTimeOut.tv_sec = seconds;      // Number of seconds
	stTimeOut.tv_usec = msec;        //   + msec milliseconds to wait

	FD_ZERO(&stReadFDS);             // Zero the set of "Read access" sockets
	FD_ZERO(&stXcptFDS);             // Zero the set of "Exception" sockets
	FD_SET(s, &stReadFDS);           // Add "s" to the set of "Read access" sockets
	FD_SET(s, &stXcptFDS);           // Add "s" to the set of "Exception" sockets

	// Check to see if Read access is enabled for socket "s"
	stat = select(-1, &stReadFDS, NULL, &stXcptFDS, &stTimeOut);
	if (stat == SOCKET_ERROR) {
		std::cout << std::endl << "wait() function failed" << std::endl;
		stat = 0;
	}
	else if (stat > 0) {
		if (FD_ISSET(s, &stXcptFDS)) {   // Some error was detected or OOB data
			stat = 0;
		}
		if (!FD_ISSET(s, &stReadFDS)) {  // No incoming data!? (check just in case)
			stat = 0;
		}
	}

	return stat;
}


int main(int argc, char* argv[]) {

	//init winsock
	WSADATA wsaData;

	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}
	// ask user whether they want to create a group or join one
	cout << "Would you like to create a group or join one? (create/join): ";
	string choice;
	cin >> choice;
	if (choice == "create") {
		//ask user for group name
		cout << "Enter a group name: ";
		string groupName;
		cin >> groupName;
		//ask user for location of study group
		cout << "Enter the location of the study group: ";
		string location;
		cin >> location;
		//ask user for courses that will be studied in the group
		cout << "Enter the courses that will be studied in the group: ";
		string courses;
		cin >> courses;

	}
	else if (choice == "join") {
		// join a group
		cout << "Joining a group..." << endl;
	}
	else {
		cout << "Invalid choice. Please enter 'create' or 'join'." << endl;
	}
	//create socket

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;


	const char* Hostname;
	const char* Port;

	if (argc == 1) {
		Hostname = (char*)"PE-HRFIELDS-1";
		Port = (char*)"17";


	}
	else if (argc == 2) {
		Hostname = argv[1];
		Port = (char*)"17";
	}
	else {
		Hostname = argv[1];
		Port = argv[2];

	}

	iResult = getaddrinfo(Hostname, Port, &hints, &result);


	if (iResult != 0) {
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	SOCKET Socket = INVALID_SOCKET;
	Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Socket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}




	if (Socket == INVALID_SOCKET) {
		cout << "Unable to connect to server" << endl;
		WSACleanup();
		return 1;
	}

	//connection has been established

	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;


	cout << "Enter command:";
	cin >> sendbuf;
	SOCKADDR_IN SenderAddr;

	//Source:
	//https://members.tripod.com/frenchwhales_site/winsockt/Lesson4.htm

	struct sockaddr_in* ipAddress = (struct sockaddr_in*)result->ai_addr;

	struct sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = ipAddress->sin_addr.s_addr;
	destAddr.sin_port = htons(17);


	int hostAddrSize = sizeof(destAddr);

	int SenderAddrSize = sizeof(SenderAddr);

	while (_stricmp(sendbuf, "quit")) {

		sendto(Socket, sendbuf, DEFAULT_BUFLEN, 0, (const sockaddr*)&destAddr, hostAddrSize);

		if (_strcmpi(sendbuf, "sendqotd") == 0) {
			while (wait(Socket, 1, 1000) == 1) {
				recvfrom(Socket, recvbuf, DEFAULT_BUFLEN, 0, (struct sockaddr*)&SenderAddr, &SenderAddrSize);
				cout << recvbuf;
			}
		}


		cout << "Enter command:";
		cin >> sendbuf;
	}


	//disconnect
	iResult = shutdown(Socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(Socket);
		WSACleanup();
		return 1;
	}

	closesocket(Socket);
	WSACleanup();

	return 0;



}