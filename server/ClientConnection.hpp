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
    int id;
    TCPSocket* sock;
    std::string address;
    unsigned short port;

    ClientConnection(int id, TCPSocket* sock);
    ~ClientConnection();

    void sendMsg(std::string message);
    std::string receiveMsg();

    bool isConnected();

private:
    bool answerWSClient(std::string msg);
    std::string translatePacket(char buffer[]);
    std::string createPacket(std::string str);
};

#endif /* CLIENTCONNECTION_H_ */
