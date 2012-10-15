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

//x position, y position, width, height
typedef std::pair <std::pair <int,int>, std::pair <int,int>> genObj; //generic object
//objects that compose the scene, such as squares
typedef std::pair <genObj, objType> gameObj; 

// x position, y position, type, client id
typedef std::pair <std::pair<int,int>, objType> bomb;
typedef std::pair <bomb, int> player;

class BomberServer: public virtual GameServer {
public:
  BomberServer();
  virtual ~BomberServer();
private:
  void step();
  void onClientConnect(ClientConnection * client);
  void onClientDisconnect(ClientConnection * client);
  void onNewMessage(std::string msg);
  void movePlayer(int);
  void newObject(int, int, int, int, objType);
  void newPlayer(int, int, objType, int);

  //  gameState gs;
  int width;
  int height;
  int xPoint;
  int yPoint;
  int xMove;
  int yMove;

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
