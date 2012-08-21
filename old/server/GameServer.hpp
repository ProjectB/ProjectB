

#ifndef GS_HPP_
#define GS_HPP_


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list>



#include "SimpleWebSocketServer.hpp" /* for TCPSocket */
#include "ClientInterface.hpp" /* to warn of a joined game */

struct gameQueue
{
  list<TCPSocket*> socketList;
};

class GameServer
{
private:
  void createGame();
  
public:
  void joinGameQueue(TCPSocket*, ClientInterface);

  GameServer();
  ~GameServer();

private:
  gameQueue game;
  list<ClientInterface> gameClients;
};





#endif /* GS_HPP_ */

