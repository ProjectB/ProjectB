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

class GameServer {

public:
	GameServer();
	virtual GameServer(int port);
    virtual ~GameServer();
private:
    GameState gs;

    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(std::string guid, std::string msg);

    virtual void step();
};

#endif /* GAMESERVER_HPP_ */
