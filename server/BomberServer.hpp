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

  gameState gs;
  int width = 640;
  int height = 480;
  

};

 private class gameState {
 public:
   typedef pair <pair<int,int>, int> gameObj;
   typedef pair <gameObj, int> player;
   vector<gameObj> objects;
   vector<player> players;
   
 private:
 }

#endif /* BOMBERSERVER_HPP_ */
