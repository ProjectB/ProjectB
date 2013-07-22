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

#include "../defs.hpp"
#include "../MultithreadQueue.hpp"
#include "ClientConnection.hpp"
#include "../util/PracticalSocket.hpp"
#include "Message.hpp"

class ConnServer : public Logger {
private:
    std::thread mainThread;
    std::atomic<bool> isRunning;
    unsigned short port;
    TCPServerSocket * servSocket;

    void run();
public:
    static MultithreadQueue<ClientConnection*> clientQueue;


    ConnServer(unsigned short port);
    void start();
    void stop();
    void runClient(ClientConnection * client);
    static bool isClientQueueEmpty();

protected:
};

#endif /* CONNSERVER_H_ */
