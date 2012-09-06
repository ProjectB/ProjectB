/*
 * ClientConnection.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */
#include <string>
#include "lib/PracticalSocket.hpp"


#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

class ClientConnection {
public:
    std::string address;
    unsigned short port;
    TCPSocket* sock;
	ClientConnection(TCPSocket* sock);
	bool isConnected();
	void sendMsg(std::string message);
	std::string receiveMsg();
private:
	bool answerWSClient(std::string msg);
	std::string translatePacket(char buffer[]);
	std::string createPacket(std::string str);
};


#endif /* CLIENTCONNECTION_H_ */
