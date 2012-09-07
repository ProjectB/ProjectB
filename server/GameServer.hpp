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
#include <string>

#include "MultithreadQueue.hpp"
#include "ClientConnection.hpp"

class GameServer {
private:
    void run();
public:
    MultithreadQueue<ClientConnection*> clientQueue;
    MultithreadQueue<std::string> msgQueue;
    std::vector<ClientConnection*> clients;
    std::vector<std::string> msgs;

    GameServer();

    void start();

    void runClient(ClientConnection * client);
};

#endif /* GAMESERVER_H_ */
