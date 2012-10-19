/*
 * GameServer.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include <queue>
#include <vector>
#include <map>
#include <string>
#include <atomic>

#include "MultithreadQueue.hpp"
#include "ClientConnection.hpp"

class GameServer {
private:
    std::thread mainThread;
    std::atomic<bool> isRunning;
    void run();
    void runClient(ClientConnection * client);
public:
    MultithreadQueue<ClientConnection*> clientQueue;
    MultithreadQueue<std::pair<std::string, std::string>> guidMsgQueue;
    std::map<std::string, ClientConnection*> clients;

    GameServer();
    virtual ~GameServer();

    void start();
    void stop();

protected:
    void broadcast(std::string msg);
    virtual void step() = 0;
    virtual void onClientConnect(ClientConnection * client) = 0;
    virtual void onClientDisconnect(ClientConnection * client) = 0;
    virtual void onNewMessage(std::string guid, std::string msg) = 0;

};

#endif /* GAMESERVER_H_ */
