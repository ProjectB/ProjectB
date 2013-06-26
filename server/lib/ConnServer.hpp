/*
 * GameServer.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef CONNSERVER_H_
#define CONNSERVER_H_

#include <queue>
#include <vector>
#include <map>
#include <string>
#include <atomic>
#include <iostream>
#include <sstream>
#include <mutex>
#include <vector>
#include <thread>
#include <cstring>

#include "Server.hpp"
#include "MultithreadQueue.hpp"
#include "ClientConnection.hpp"

class ConnServer {
private:
    std::thread mainThread;
    std::thread stepThread;
    std::atomic<bool> isRunning;
    int clientsOnline;

    //long
    void run();
public:
    std::map<std::string, ClientConnection*> clients;

    ConnServer();
    ConnServer(int port);
    virtual ~ConnServer();

    void start();
    void stop();

protected:
    void broadcast(std::string msg);
    virtual void step() = 0;
    virtual void onClientConnect(ClientConnection * client) = 0;
    virtual void onClientDisconnect(ClientConnection * client) = 0;
    virtual void onNewMessage(std::string guid, std::string msg) = 0;
    Server * server;

};

#endif /* CONNSERVER_H_ */
