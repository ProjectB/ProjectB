/*
 *
 * Bomber.hpp
 *
 */


#ifndef BOMBER_HPP_
#define BOMBER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <list>



#include "SimpleWebSocketServer.hpp" /* for TCPSocket */
#include "GameServer.hpp"


struct Player
{
  TCPSocket* socket;
  string nickname;
  //    unsigned int level;
  //    unsigned int id;
  
};



class Bomber
{
public:
  Bomber(gameQueue);
  ~Bomber();

private:
  Player p1, p2, p3, p4;
};


#endif /* BOMBER_HPP_ */
