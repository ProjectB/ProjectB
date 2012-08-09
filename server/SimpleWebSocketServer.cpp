/*
 * SimpleWebSocketServer.cpp
 *
 *  Created on: Aug 8, 2012
<<<<<<< HEAD
 *      Author: ricardo
 */

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
=======
 *      Author: ricardo, the wizard
 */

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include "SHA1.h"			  // For SHA-1 encoding
>>>>>>> kodo
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <cstring>
#include <sstream>

using namespace std;

const unsigned int RCVBUFSIZE = 32;    // Size of receive buffer

void HandleTCPClient(TCPSocket *sock); // TCP client handling function

int main(int argc, char *argv[]) {

	unsigned short echoServPort = 8080;  // First arg: local port

	if (argc >= 2) {                     // Test for correct number of arguments
		echoServPort = atoi(argv[1]);  // First arg: local port
	}

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
	} catch (const SocketException & e) {
		cerr << "Unable to get foreign address" << endl;
	}
	try {
		cout << sock->getForeignPort();
	} catch (const SocketException & e) {
		cerr << "Unable to get foreign port" << endl;
	}
	cout << endl;

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];
	unsigned int recvMsgSize;
	stringstream buffer;
	while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0) { // Zero means
		// end of transmission
		//cout << echoBuffer;
		int pos;
		if ((pos = string(echoBuffer).find("\r\n\r\n")) != string::npos)
		{
			echoBuffer[pos] = 0;
			buffer << echoBuffer;
			break;
		}

		buffer << echoBuffer;

	}
	//cout << endl;
	cout << buffer.str() << endl;
<<<<<<< HEAD

=======
	
	char answer[256];
	string tempAnswer;
	int begin, end;
	int correction;

	
	
	
	begin = string(buffer.str()).find("Sec-WebSocket-Key");
	begin += 19; //sizeof("Sec-WebSocket-Key: ") = 20..19 :D
	
	end = string(buffer.str()).find("Sec-WebSocket-Version");

	end -= 2; // sizeof("\r\n") = 3..2 :D

	
	buffer.str().copy(answer, end - begin, begin);

	answer[end-begin] = '\0';

	cout << "answer: " << answer << "!!!" << endl;

	strcat(answer, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11\0");

	cout << "new answer: " << answer << "!!!" << endl;
	

	/*TO DO: SHA-1 esse answer... 
	  eh algo do tipo
	  
	  sha1.Update((UINT_8*)str.c_str(), str.size() * sizeof(TCHAR));
	  sha1.Final();
	  sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	  tcout << _T("String hashed to:") << endl;
	  tcout << strReport << endl;
	  
	  tenho aula daqui 6hrs, melhor eu dormir xD

	*/
	
	
	/*
	HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=
Sec-WebSocket-Protocol: chat


	tempAnswer << "HTTP/1.1 101 Switching" << endl << "Upgrade: websocket" << endl << "Connection: Upgrade" << endl;
	

	
GET / HTTP/1.1
Upgrade: websocket
Connection: Upgrade
Host: localhost:8080
Origin: null
Sec-WebSocket-Key: +eoe548nif9s7FtAy5lJOQ==
Sec-WebSocket-Version: 13
Sec-WebSocket-Extensions: x-webkit-deflate-frame

*/
	
	
>>>>>>> kodo
	//string answer("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html;\r\ncharset=UTF-8\r\n\r\nFuncionou!\r\n\r\n");
	//sock->send(answer.c_str(), strlen(answer.c_str()));

	delete sock;
}
