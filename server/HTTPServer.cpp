/*
 * HTTPServer.cpp
 *
 *  Created on: Aug 8, 2012
 *      Author: ricardo
 */

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <cstring>

using namespace std;

const unsigned int RCVBUFSIZE = 32;    // Size of receive buffer

void HandleTCPClient(TCPSocket *sock); // TCP client handling function

int main(int argc, char *argv[]) {

	unsigned short echoServPort = 8080;  // First arg: local port

	if (argc >= 2) {                     // Test for correct number of arguments
		echoServPort = atoi(argv[1]);  // First arg: local port
		//cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
		//exit(1);
	}

	//unsigned short echoServPort = atoi(argv[1]);  // First arg: local port

	cout << "Listening at port " << echoServPort << endl;

	try {
		TCPServerSocket servSock(echoServPort);     // Server Socket object

		for (;;) {   // Run forever
			HandleTCPClient(servSock.accept());  // Wait for a client to connect
		}
	} catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
	// NOT REACHED

	return 0;
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {
	cout << "Handling client ";
	try {
		cout << sock->getForeignAddress() << ":";
	} catch (SocketException e) {
		cerr << "Unable to get foreign address" << endl;
	}
	try {
		cout << sock->getForeignPort();
	} catch (SocketException e) {
		cerr << "Unable to get foreign port" << endl;
	}
	cout << endl;

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];
	unsigned int recvMsgSize;
	while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0) { // Zero means
		// end of transmission

		if (recvMsgSize < RCVBUFSIZE)
			echoBuffer[recvMsgSize] = 0;
		cout << echoBuffer;
		if (string(echoBuffer).find("\r\n\r\n") != string::npos)
			break;
	}
	cout << endl;

	string answer("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html;\r\ncharset=UTF-8\r\n\r\nFuncionou!\r\n\r\n");
	sock->send(answer.c_str(), strlen(answer.c_str()));

	delete sock;
}
