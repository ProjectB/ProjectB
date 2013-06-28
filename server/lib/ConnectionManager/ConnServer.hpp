/*
 * ConnServer.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef CONNSERVER_H_
#define CONNSERVER_H_

#include <atomic>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>

#include "../MultithreadQueue.hpp"
#include "ClientConnection.hpp"
#include "../util/PracticalSocket.hpp"

class ConnServer : public Logger {
private:
    std::thread mainThread;
    std::atomic<bool> isRunning;
    TCPServerSocket * servSocket;
    unsigned short port;
    void run();
public:
    ConnServer(unsigned short port);
    void start();
    void stop();
    MultithreadQueue<ClientConnection*> clientQueue;
    void runClient(ClientConnection * client);
};

#endif /* CONNSERVER_H_ */
