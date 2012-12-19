/*
 * Server.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <atomic>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>

#include "MultithreadQueue.hpp"
#include "ClientConnection.hpp"
#include "util/PracticalSocket.hpp"

class Server : public Logger {
private:
    std::thread mainThread;
    std::atomic<bool> isRunning;
    TCPServerSocket * servSocket;
    unsigned short port;
    void run();
public:
    Server(unsigned short port);
    void start();
    void stop();
    MultithreadQueue<ClientConnection*> clientQueue;
    void runClient(ClientConnection * client);
};

#endif /* SERVER_H_ */
