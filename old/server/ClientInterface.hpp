
#ifndef CL_INTER_HPP_
#define CL_INTER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



#include "SimpleWebSocketServer.hpp"

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


#endif /* CL_INTER_HPP_ */
