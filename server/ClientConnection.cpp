#include <iostream>
#include <sstream>
#include <string>
#include "lib/PracticalSocket.h"
#include "ClientConnection.h"

ClientConnection::ClientConnection(TCPSocket* sock)
{
	const unsigned int RECVBUFSIZE = 32;

	cout << "Handling client ";
	try {
		cout << sock->getForeignAddress() << ":";
	} catch (const SocketException& e) {
		cerr << "Unable to get foreign address" << endl;
	}
	try {
		cout << sock->getForeignPort();
	} catch (const SocketException& e) {
		cerr << "Unable to get foreign port" << endl;
	}
	cout << endl;

	char recvBuffer[RECVBUFSIZE];
	unsigned int recvMsgSize;
	stringstream msg;

	while ((recvMsgSize = sock->recv(recvBuffer, RECVBUFSIZE)) > 0) {
		int pos;
		if ((pos = string(recvBuffer).find("\r\n\r\n")) != string::npos)
		{
			recvBuffer[pos] = 0;
			msg << recvBuffer;
			break;
		}
		msg << recvBuffer;
	}

	answerWSClient(sock, msg.str());
}



void ClientConnection::answerWSClient(TCPSocket* sock, string msg)
{

}

