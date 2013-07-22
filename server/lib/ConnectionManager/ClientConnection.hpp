/*
 * ClientConnection.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include <atomic>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "../MultithreadQueue.hpp"
#include "../util/PracticalSocket.hpp"
#include "../Logger.hpp"
#include "Message.hpp"

enum FrameType {
    TEXT,
    BIN,
    CONTROL,
    CLOSE,
    PING,
    PONG
};

class ClientConnection : public Logger {
public:
    int id;
    std::string guid;
    std::string address;
    unsigned short port;
    std::mutex connMutex;
    MultithreadQueue<Message> msgQueue;
    unsigned int messageCounter;

    ClientConnection(int id, TCPSocket* sock);
    ~ClientConnection();

    std::string str();
    bool isConnected();
    void disconnect();
    void sendMsg(std::string message);
    void receiveMsg(std::vector<Message>& msgs);
    int hasData(int sec = 0, int usec = 0);
    void start();
    void stop();


private:
    std::thread mainThread;
    std::atomic<bool> isRunning;
    TCPSocket* sock;
    bool answerWSClient(std::string msg);
    std::string createPacket(std::string str);
    void updateRcv(unsigned int& pos, void *buffer, bool block = true);
    void run();

};

#endif /* CLIENTCONNECTION_H_ */
