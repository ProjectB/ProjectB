#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <thread>



#include "PracticalSocket.h"
#include "SHA1.h"
#include "base64.h"

using namespace std;

/* BEGIN: SimpleWebSocketServer */

const unsigned int RCVBUFSIZE = 32;

class SimpleWebSocketServer
{
private:
  inline const unsigned int rol(const unsigned int, const unsigned int);
  void innerHash(unsigned int *, unsigned int *);
  void calc(const void *, const int, unsigned char *);
  void toHexString(const unsigned char *, char *);
  
public:
  void handleTCPClient(TCPSocket *);
};

/* END: SimpleWebSocketServer */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* BEGIN: ClientInterface */

class ClientInterface
{
private:
  string receivePacket();
  string translatePacket(char*);
  void sendToServer(string);
  
  
public:
  void unqueue();
  ClientInterface(TCPSocket*);
  ~ClientInterface();
  
  
  
private:
  TCPSocket* socket;
  bool isQueued;
  
};


/* END: ClientInterface */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* BEGIN: GameServer */

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

/* END: GameServer */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* BEGIN: Bomber  */

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


/* END: Bomber */
