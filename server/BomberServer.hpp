/*
 * BomberServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef BOMBERSERVER_HPP_
#define BOMBERSERVER_HPP_

#include "GameServer.hpp"


enum objType
  {
    square,
    bomber
  };

typedef std::pair <std::pair<int,int>, objType> gameObj;
typedef std::pair <gameObj, int> player;

class BomberServer: public virtual GameServer {
public:
    BomberServer();
    virtual ~BomberServer();
private:
    void step();
    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(std::string msg);

  //  gameState gs;
  int width;
  int height;

  std::vector<gameObj> objects;
  std::vector<player> players;

  

};

/*
 private class gameState {
 public:
   
 private:
 }

*/

#endif /* BOMBERSERVER_HPP_ */
