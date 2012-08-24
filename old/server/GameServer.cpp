/*
 * 
 * GameServer.cpp
 *
 */

#include "defines.hpp"

#define _GSDEBUG 0

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}


void GameServer::joinGameQueue(TCPSocket* sock, ClientInterface clInt)
{
  game.socketList.push_back(sock);
  gameClients.push_back(clInt);

  if((int) game.socketList.size() >= 4)
    {
      /* not very smart :D */
      createGame();
    }

}

void callGame(gameQueue localQueue)
{
  Bomber *bomb = new Bomber(localQueue);

  delete bomb;
}

void GameServer::createGame()
{
  gameQueue localQueue;
  int i;


  /* tell clients to unqueue */
  for(i=0; i < 4; i++)
    {
      ((ClientInterface)game.socketList.front()).unqueue();
      game.socketList.pop_front();
    }

  /* copy clients into a local queue */
  for(i=0; i < 4; i++)
    {
      localQueue.socketList.push_back(game.socketList.front());
      game.socketList.pop_front();
    }

  thread gameThread(callGame, localQueue);
  
  return;
}





