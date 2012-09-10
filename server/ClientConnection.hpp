/*
 * ClientConnection.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include <string>
#include <thread>
#include "lib/PracticalSocket.hpp"

class ClientConnection {
public:
    int id;
    std::string address;
    unsigned short port;
    std::mutex connMutex;

    ClientConnection(int id, TCPSocket* sock);
    ~ClientConnection();

    std::string str();
    bool isConnected();
    void disconnect();
    void sendMsg(std::string message);
    std::string receiveMsg();
    int hasData();

private:
    TCPSocket* sock;
    bool answerWSClient(std::string msg);
    std::string translatePacket(char buffer[]);
    std::string createPacket(std::string str);

};

#endif /* CLIENTCONNECTION_H_ */
