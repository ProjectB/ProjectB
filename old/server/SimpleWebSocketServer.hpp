
#ifndef SWSS_HPP_
#define SWSS_HPP_

#include <stdio.h>
#include <iostream>              // For cerr and cout
#include <cstdlib>               // For atoi()
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <list>



#include "PracticalSocket.h"     // For Socket, ServerSocket, and SocketException
#include "SHA1.h"                // For SHA-1 encoding
#include "base64.h"
#include "ClientInterface.hpp"

using namespace std;


const unsigned int RCVBUFSIZE = 32;        // Size of receive buffer

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

#endif /* SWSS_HPP_ */
