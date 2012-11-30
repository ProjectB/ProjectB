/*
 * ClientConnection.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "util/PracticalSocket.hpp"

enum FrameType {
    TEXT,
    BIN,
    CONTROL,
    CLOSE,
    PING,
    PONG
};

class ClientConnection {
public:
    int id;
    std::string guid;
    std::string address;
    unsigned short port;
    std::mutex connMutex;

    ClientConnection(int id, TCPSocket* sock);
    ~ClientConnection();

    std::string str();
    bool isConnected();
    void disconnect();
    void sendMsg(std::string message);
    void receiveMsg(std::vector<std::string>& msgs);
    int hasData(int sec = 0, int usec = 0);

private:
    TCPSocket* sock;
    bool answerWSClient(std::string msg);
    std::string createPacket(std::string str);
    void updateRcv(unsigned int& pos, void *buffer, bool block = true);
    void log(std::string msg);

};

#endif /* CLIENTCONNECTION_H_ */