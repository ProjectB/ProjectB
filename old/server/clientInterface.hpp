
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#ifndef CL_INTER_HPP_
#define CL_INTER_HPP_

#include "SimpleWebSocketServer.hpp"

class clientInterface
{
private:
  string receivePacket();
  string translatePacket(char*);
  void sendInput(string);
  
  
public:
  clientInterface(TCPSocket*);
  ~clientInterface();
  

private:
  TCPSocket* socket;  
  
};


#endif /* CL_INTER_HPP_ */
