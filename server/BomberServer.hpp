/*
 * BomberServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef BOMBERSERVER_HPP_
#define BOMBERSERVER_HPP_

#include <iostream>
#include <sstream>
#include "defs.hpp"
#include "lib/GameServer.hpp"
#include "GameState.hpp"

class BomberServer: public virtual GameServer {

public:
    BomberServer();
    BomberServer(int port);
    virtual ~BomberServer();
private:
    GameState gs;

    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(std::string guid, std::string msg);
    void step();
};

#endif /* BOMBERSERVER_HPP_ */
