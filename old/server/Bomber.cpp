/*
 *
 * Bomber.cpp
 *
 */


#include "Bomber.hpp"


#define _BOMBER_DEBUG 0

Bomber::Bomber(gameQueue socks)
{
  p1.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p1.nickname = "Player 1";

  p2.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p2.nickname = "Player 2";

  p3.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p3.nickname = "Player 3";

  p4.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p4.nickname = "Player 4";

  /* start game */
}

Bomber::~Bomber()
{
  delete &p1;
  delete &p2;
  delete &p3;
  delete &p4;
  
}


