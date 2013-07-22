/*
 * BomberServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef GAMESERVER_HPP_
#define GAMESERVER_HPP_

#include <iostream>
#include <sstream>
#include "../defs.hpp"
#include "GameState.hpp"
#include "../ConnectionManager/ClientConnection.hpp"
#include "../ConnectionManager/Message.hpp"
#include "../ConnectionManager/ConnServer.hpp"

class GameServer{
public:
	GameServer();
    virtual ~GameServer();

    void start();
    void stop();
private:
    GameState gs;
    std::thread mainThread;
    std::thread stepThread;
    std::map<std::string, ClientConnection*> clients;
    MultithreadQueue<Message*> messageQueue;

    bool isRunning;

    void receiveClientMessages(ClientConnection *);
    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(Message* m);
    void broadcast(std::string msg);
    void run();

    virtual void step();
};

#endif /* GAMESERVER_HPP_ */
