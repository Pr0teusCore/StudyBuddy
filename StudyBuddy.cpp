#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
using std::cin;
using std::cout;
using std::endl;
using std::string;
#pragma comment(lib, "Ws2_32.lib")
int main(int argc, char* argv[]) {
	char HostName[100] = "PE-HRFIELDS-1";
	char ServiceName[100] = "7";
	if (argv[1] == NULL) {
		argv[1] = HostName;
	}
	if (argv[2] == NULL) {
		argv[2] = ServiceName;
	}
	//PE-HRFIELDS-1
	//initialize WS2
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		cout << "don goofed" << iResult;
		return 1;
	}
	//create socket
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; //SOCK_DGRAM FOR CONNECTIONLESS
	hints.ai_protocol = IPPROTO_TCP; //GUESS WHAT FOR CONNECTIONLESS

	//resolve servername+port
	//determine service name/port
	iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (iResult != 0) {
		cout << "don goofed 2: getaddrinfo failed";
		WSACleanup();
		return 1;
	}

	//create socket
	SOCKET ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		cout << "don goofed 3: error at socket" << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	//connect
	iResult = connect(ConnectSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "don goofed 4: connection failed" << WSAGetLastError;
		closesocket(ConnectSocket);
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else if (ConnectSocket == INVALID_SOCKET) {
		cout << "unable to connect";
		freeaddrinfo(result);
		WSACleanup();
	}
	const int defaultbuf = 512;
	//established connection!!!
	//send
	char buffer[100] = "";
	cout << "Message to send:";

	cin.getline(buffer, 100);
	if (buffer == NULL) {

	}
	else {
		//convert char* to string

		while (strcmp(buffer, "quit") || strcmp(buffer, "QUIT") || strcmp(buffer, "Quit")) {
			cout << "Message to send:";
			char recvbuf[512];
			int recvbuflen = defaultbuf;
			iResult = send(ConnectSocket, buffer, (int)strlen(buffer), 0);
			if (iResult == SOCKET_ERROR) {
				cout << "don goofed 5";
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
			cout << "Bytes sent: " << iResult << endl;

			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				string s = string(recvbuf);
				cout << "Message Received:" << s << endl;
				cout << "Bytes received:" << iResult << endl;
			}
			else if (iResult == 0) {
				cout << "Connection closed";
			}
			else {
				cout << "recv failed:" << WSAGetLastError();
			}
			for (size_t i = 0; i < 100; i++)
			{
				recvbuf[i] = NULL;
			}
			cin.getline(buffer, 100);
		}
	}
	//shutdown
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "don goofed 6";
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}



}