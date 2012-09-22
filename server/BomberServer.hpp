/*
 * BomberServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef BOMBERSERVER_HPP_
#define BOMBERSERVER_HPP_

#include "GameServer.hpp"

class BomberServer: public virtual GameServer {
public:
    BomberServer();
    virtual ~BomberServer();
private:
    void step();
    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(std::string msg);
};

#endif /* BOMBERSERVER_HPP_ */
